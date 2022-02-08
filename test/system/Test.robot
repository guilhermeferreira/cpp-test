*** Settings ***
Documentation  QIA v1
Resource  QNodeOS
Resource  NetQASM
Resource  Host

# Setup - ./platforms/qemu/tests.sh -u --host-ip ${HOST_IP} --qnos-ip ${QNOS_IP}
# TearDown - ./platforms/qemu/tests.sh -d --host-ip ${HOST_IP} --qnos-ip ${QNOS_IP}
# -v VAR_1:"Value 1" -v VAR_2:"value 2"
Suite Setup  
Suite Teardown  
Test Setup  
Test Teardown  

*** Variables ***
# RECEIVE AS ARGUMENTS
${QNOS_IP} =  192.168.2.215
${HOST_IP} =  192.168.2.210
${QMOCK_PORT} =  6666
${TEST_APP} =  bqc_5_1
@{TEST_APP_LIST} =  bqc_5_1  bqc_5_2  bqc_5_3

*** Test Cases ***
Single Node 1 Test Case
    [Documentation]  Test standalone QNodeOS
    [Tags]  smoke
    ${BUID_TYPE} =  Debug
    # QNodeOS.Run builds a new QNodeOS with different IP
    # BUID_TYPE is Debug by default
    QNodeOS.Run  ${HOST_IP}  ${QNOS_IP}  ${QMOCK_PORT}  ${BUID_TYPE}
    NetQASM.Execute  @{TEST_APP_LIST}[0]  ${QNOS_IP}

# Single Node 2 Test Case
#     [Documentation]  Test QNodeOS with CNet
#     [Tags]  smoke
#     ${BUID_TYPE} =  Release
#     Run CNet  ${HOST_IP} ${QNOS_IP}
#     QNodeOS.Run   ${HOST_IP}  ${QNOS_IP}  ${QMOCK_PORT}  ${BUID_TYPE}
#     NetQASM.Execute  @{TEST_APP_LIST}[1]  ${QNOS_IP}

# Two Nodes Test Case
#     [Documentation]  Test two QNOdeOS
#     [Tags]  compatibility
#     QNodeOS.Run  ${HOST1_IP}  ${QNOS1_IP}  ${QMOCK1_PORT}
#     QNodeOS.Run  ${HOST2_IP}  ${QNOS2_IP}  ${QMOCK2_PORT}
#     NetQASM.Execute  ${TEST_APP}  ${QNOS1_IP}
#     NetQASM.Execute  ${TEST_APP}  ${QNOS2_IP}

*** Keywords ***
