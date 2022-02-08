import os

class Host(object):

    def __init__(self):
        print("Host created")

    def iface_up(self, host_ip, qnos_ip):
        cmd = '/usr/src/QNodeOS/platforms/qemu/tests.sh -u --host-ip {host_ip} --qnos-ip {qnos_ip}'
        cmd.format(host_ip=host_ip, qnos_ip=qnos_ip)
        self._run_command(cmd)

    def iface_down(self, host_ip, qnos_ip):
        cmd = '/usr/src/QNodeOS/platforms/qemu/tests.sh -d --host-ip {host_ip} --qnos-ip {qnos_ip}'
        cmd.format(host_ip=host_ip, qnos_ip=qnos_ip)
        self._run_command(cmd)

    def _run_command(self, command, *args):
        stream = os.popen(command)
        output = stream.read()
        return output
