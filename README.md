Oculus Phantom Touch Remover
By Kojack.

Recently the Oculus Rift software has been adding phantom touch controllers to the device list. These fake controllers can't be removed using the desktop client.
These are caused by faulty entries in the DeviceCache.json file.

To remove these phantom controllers, do the following:
1 - Use the Windows Task Manager (service tab) to stop the OVRService.
2 - Run OculusPhantomTouchRemover.exe.
3 - Use Task Manager to start the OVRService again.

All phantom controllers should now be removed without affecting other configured devices.
