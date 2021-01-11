Patch.o: Patch.c Patch.h global.h ipc.h syscalls.h string.h common.h \
 ../fatfs/ff.h ../fatfs/integer.h ../fatfs/ffconf.h vsprintf.h dol.h \
 elf.h PatchCodes.h asm/EXIImm.h asm/EXISelect.h asm/EXILock.h \
 asm/EXIDMA.h asm/EXIProbe.h asm/EXIGetID.h asm/__CARDReadStatus.h \
 asm/__CARDClearStatus.h asm/ReadROM.h asm/ARQPostRequest.h \
 asm/ARStartDMA.h asm/ARStartDMA_PM.h asm/ARStartDMA_TC.h \
 asm/ARStartDMA_Hook.h asm/__ARHandler.h asm/SITransfer.h asm/SIGetType.h \
 asm/FakeInterrupt.h asm/FakeInterrupt_DBG.h asm/FakeInterrupt_Datel.h \
 asm/__DVDInterruptHandler.h asm/TCIntrruptHandler.h \
 asm/SIIntrruptHandler.h asm/SIInitStore.h asm/PADRead.h \
 asm/PADControlAllMotors.h asm/PADControlMotor.h asm/PADIsBarrel.h \
 asm/DVDSendCMDEncrypted.h asm/GCAMSendCommand.h asm/patch_fwrite_Log.h \
 asm/patch_fwrite_GC.h asm/FakeRSWLoad.h asm/FakeRSWStore.h \
 asm/FakeEntryLoad.h asm/SwitcherPrs.h asm/OSReportDM.h \
 asm/OSExceptionInit.h asm/__DSPHandler.h asm/__GXSetVAT.h \
 asm/GXInitTlutObj.h asm/GXLoadTlut.h asm/DatelTimer.h \
 asm/SonicRidersCopy.h asm/MajoraAudioStream.h asm/MajoraLoadRegs.h \
 asm/MajoraSaveRegs.h asm/codehandler_stub.h asm/SOInit.h asm/SOStartup.h \
 asm/SOCleanup.h asm/SOSocket.h asm/SOClose.h asm/SOListen.h \
 asm/SOAccept.h asm/SOBind.h asm/SOShutdown.h asm/SORecvFrom.h \
 asm/SOSendTo.h asm/SOSetSockOpt.h asm/SOFcntl.h asm/SOPoll.h \
 asm/IPGetMacAddr.h asm/IPGetNetmask.h asm/IPGetAddr.h asm/IPGetMtu.h \
 asm/IPGetLinkState.h asm/IPGetConfigError.h asm/IPSetConfigError.h \
 asm/IPClearConfigError.h asm/HSPIntrruptHandler.h asm/avetcp_init.h \
 asm/avetcp_term.h asm/dns_set_server.h asm/dns_clear_server.h \
 asm/dns_open_addr.h asm/dns_get_addr.h asm/dns_close.h \
 asm/DHCP_request_nb.h asm/DHCP_get_gateway.h asm/DHCP_get_dns.h \
 asm/DHCP_release.h asm/tcp_create.h asm/tcp_bind.h asm/tcp_listen.h \
 asm/tcp_stat.h asm/tcp_getaddr.h asm/tcp_connect.h asm/tcp_accept.h \
 asm/tcp_send.h asm/tcp_receive.h asm/tcp_abort.h asm/tcp_delete.h \
 asm/Return0.h asm/Return1.h asm/ReturnMinus1.h asm/KeyboardRead.h \
 PatchWidescreen.h PatchTimers.h TRI.h Config.h alloc.h \
 ../common/include/CommonConfig.h ../common/include/NintendontVersion.h \
 ../common/include/Metadata.h patches.c DI.h ISO.h SI.h EXI.h sock.h \
 ../codehandler/codehandler.h ../codehandler/codehandleronly.h \
 ../fatfs/ff_utf8.h ../fatfs/ff.h
Patch.h:
global.h:
ipc.h:
syscalls.h:
string.h:
common.h:
../fatfs/ff.h:
../fatfs/integer.h:
../fatfs/ffconf.h:
vsprintf.h:
dol.h:
elf.h:
PatchCodes.h:
asm/EXIImm.h:
asm/EXISelect.h:
asm/EXILock.h:
asm/EXIDMA.h:
asm/EXIProbe.h:
asm/EXIGetID.h:
asm/__CARDReadStatus.h:
asm/__CARDClearStatus.h:
asm/ReadROM.h:
asm/ARQPostRequest.h:
asm/ARStartDMA.h:
asm/ARStartDMA_PM.h:
asm/ARStartDMA_TC.h:
asm/ARStartDMA_Hook.h:
asm/__ARHandler.h:
asm/SITransfer.h:
asm/SIGetType.h:
asm/FakeInterrupt.h:
asm/FakeInterrupt_DBG.h:
asm/FakeInterrupt_Datel.h:
asm/__DVDInterruptHandler.h:
asm/TCIntrruptHandler.h:
asm/SIIntrruptHandler.h:
asm/SIInitStore.h:
asm/PADRead.h:
asm/PADControlAllMotors.h:
asm/PADControlMotor.h:
asm/PADIsBarrel.h:
asm/DVDSendCMDEncrypted.h:
asm/GCAMSendCommand.h:
asm/patch_fwrite_Log.h:
asm/patch_fwrite_GC.h:
asm/FakeRSWLoad.h:
asm/FakeRSWStore.h:
asm/FakeEntryLoad.h:
asm/SwitcherPrs.h:
asm/OSReportDM.h:
asm/OSExceptionInit.h:
asm/__DSPHandler.h:
asm/__GXSetVAT.h:
asm/GXInitTlutObj.h:
asm/GXLoadTlut.h:
asm/DatelTimer.h:
asm/SonicRidersCopy.h:
asm/MajoraAudioStream.h:
asm/MajoraLoadRegs.h:
asm/MajoraSaveRegs.h:
asm/codehandler_stub.h:
asm/SOInit.h:
asm/SOStartup.h:
asm/SOCleanup.h:
asm/SOSocket.h:
asm/SOClose.h:
asm/SOListen.h:
asm/SOAccept.h:
asm/SOBind.h:
asm/SOShutdown.h:
asm/SORecvFrom.h:
asm/SOSendTo.h:
asm/SOSetSockOpt.h:
asm/SOFcntl.h:
asm/SOPoll.h:
asm/IPGetMacAddr.h:
asm/IPGetNetmask.h:
asm/IPGetAddr.h:
asm/IPGetMtu.h:
asm/IPGetLinkState.h:
asm/IPGetConfigError.h:
asm/IPSetConfigError.h:
asm/IPClearConfigError.h:
asm/HSPIntrruptHandler.h:
asm/avetcp_init.h:
asm/avetcp_term.h:
asm/dns_set_server.h:
asm/dns_clear_server.h:
asm/dns_open_addr.h:
asm/dns_get_addr.h:
asm/dns_close.h:
asm/DHCP_request_nb.h:
asm/DHCP_get_gateway.h:
asm/DHCP_get_dns.h:
asm/DHCP_release.h:
asm/tcp_create.h:
asm/tcp_bind.h:
asm/tcp_listen.h:
asm/tcp_stat.h:
asm/tcp_getaddr.h:
asm/tcp_connect.h:
asm/tcp_accept.h:
asm/tcp_send.h:
asm/tcp_receive.h:
asm/tcp_abort.h:
asm/tcp_delete.h:
asm/Return0.h:
asm/Return1.h:
asm/ReturnMinus1.h:
asm/KeyboardRead.h:
PatchWidescreen.h:
PatchTimers.h:
TRI.h:
Config.h:
alloc.h:
../common/include/CommonConfig.h:
../common/include/NintendontVersion.h:
../common/include/Metadata.h:
patches.c:
DI.h:
ISO.h:
SI.h:
EXI.h:
sock.h:
../codehandler/codehandler.h:
../codehandler/codehandleronly.h:
../fatfs/ff_utf8.h:
../fatfs/ff.h:
