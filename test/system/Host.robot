*** Settings ***
Documentation  Host Library
Library        lib/Host.py

*** Variables ***


*** Test Cases ***


*** Keywords ***
Interface Up
    [Documentation]  Run NetQASM Application
    [Arguments]      ${HOST_IP}  ${QNOS_IP}
    iface up         ${HOST_IP}  ${QNOS_IP}

Interface Down
    [Documentation]  Run NetQASM Application
    [Arguments]      ${HOST_IP}  ${QNOS_IP}
    iface down       ${HOST_IP}  ${QNOS_IP}
