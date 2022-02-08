import argparse
import time
from qdevice import QDevice


def status_code_to_str(status: int):
    # Try to convert status code to string, if the status code is a value of QDevice.Status.
    # Otherwise, convert to hexadecimal representation.
    try:
        status_str = QDevice.Status(status).name
    except ValueError:
        status_str = '0x{:02X}'.format(status)
    return status_str


def outcome_code_to_str(outcome: int):
    # Try to convert outcome code to string, if the outcome code is a value of QDevice.Outcome.
    # Otherwise, convert to hexadecimal representation.
    try:
        outcome_str = QDevice.Outcome(outcome).name
    except ValueError:
        outcome_str = '0x{:02X}'.format(outcome)
    return outcome_str


def qdevice_transaction(qdevice: QDevice, no_log_nops=False):
    try:
        sent, rcvd = qdevice.send_recv_msg()
        if no_log_nops and rcvd['operation'] == QDevice.CmdType.NOP:
            return
        if rcvd['operation'] == QDevice.CmdType.ENT or \
           rcvd['operation'] == QDevice.CmdType.ENM:
           qdevice.outcome = (QDevice.Outcome.OK | QDevice.EntState.PSI_PLUS)
        else:
            qdevice.outcome = QDevice.Outcome.OK
        qdevice.print_last_msg()
    except Exception as e:
        print(str(e))


def main():
    ################################################################################################
    # Setup
    ################################################################################################

    parser = argparse.ArgumentParser()

    # Initial value of cycle count (default: cc_init=0)
    parser.add_argument(
            '-c',
            '--cc-init',
            action='store',
            default=0,
            type=int,
            help='initial value of cycle count')

    # Number of cycle counts excluding sync (default: cc_total=32)
    parser.add_argument(
            '-t',
            '--cc-total',
            action='store',
            default=32,
            type=int,
            help='total number of cycle counts excluding sync')

    # Interactive mode (default: interactive=False)
    parser.add_argument(
            '-i',
            '--interactive',
            action='store_true',
            default=False,
            help='enable interactive mode')

    # Do not log NOPs (default: no_log_nops=False)
    parser.add_argument(
            '-n',
            '--no-log-nops',
            action='store_true',
            default=False,
            help='disable logging of NOPs')

    args = parser.parse_args()

    # Open connection with AD2 device and set-up QDevice interface protocol.
    qdevice = QDevice()

    qdevice.cycle_count = args.cc_init

    ################################################################################################
    # Main loop
    ################################################################################################

    # Let QNodeOS sync with cycle count.
    if qdevice.cycle_count < 32:
        for i in range(0, 32):
            time.sleep(0.1)
            try:
                qdevice.send_recv_msg()
            except Exception as e:
                print(str(e))

    # Set default QDevice status and outcome to send.
    qdevice.reset_stats()
    qdevice.status = QDevice.Status.OUTCOME_READY
    qdevice.outcome = QDevice.Outcome.OK

    if args.interactive:

        # Print help message for interactive mode.
        print('Interactive mode.')
        print('- Enter e.g. "ST=0x01 OC=0x0C" to change QDevice status and outcome')
        print('  (https://3.basecamp.com/3321769/buckets/14171081/documents/3185940630)')
        print('- Enter a blank input string to not change status nor outcome')
        print('- Enter "s" to stop')

        status_str = status_code_to_str(qdevice.status)
        outcome_str = outcome_code_to_str(qdevice.outcome)

        while True:

            # Ask for user input (status and outcome codes, or stop).
            previous_fields = f'ST={status_str}, OC={outcome_str}'
            user_input = input(f'Enter QDevice message fields (or blank for {previous_fields}): ')
            # Delete last message from stdout.
            # https://stackoverflow.com/questions/12586601/remove-last-stdout-line-in-python
            print('\x1b[1A' + '\x1b[2K' + '\x1b[1A')

            input_args = user_input.split()

            # If the input contains "s", break loop.
            if 's' in input_args:
                break

            # If the input contains "ST=<status>", change QDevice status.
            status = [option for option in input_args if 'ST' in option]
            if len(status):
                qdevice.status = int(status[0].split('=')[1], 0)
                status_str = status_code_to_str(qdevice.status)

            # If the input contains "OC=<outcome>", change QDevice outcome.
            outcome = [option for option in input_args if 'OC' in option]
            if len(outcome):
                qdevice.outcome = int(outcome[0].split('=')[1], 0)
                outcome_str = outcome_code_to_str(qdevice.outcome)

            qdevice_transaction(qdevice, args.no_log_nops)

    else:
        for i in range(0, args.cc_total):
            time.sleep(0.1)
            qdevice_transaction(qdevice, args.no_log_nops)

    ################################################################################################
    # Finalization
    ################################################################################################

    # Print stats at the end.
    print('{}'.format('=' * 100))
    qdevice.print_stats()


if __name__ == '__main__':
    main()
