### Creating a New Version & Installer ###

- First, a static Qt build is needed to generate a .exe to put into the installer.<BR>
  Follow these instructions, note that the script takes ages (more than a working day even on a powerful machine):<BR>
  [https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW](https://wiki.qt.io/Building_a_static_Qt_for_Windows_using_MinGW)
  
- If not already installed, install the Qt Installer Framework:<BR>
  [https://wiki.qt.io/Qt-Installer-Framework](https://wiki.qt.io/Qt-Installer-Framework)

- The Mercury Test Utility uses the latest Git tag to extract the version code.<BR>
  Ensure that the tip of the branch you are building from has a Git tag of the exact format
  major.minor.build<BR>
  e.g. 1.1.0
  
- Run the statically-linked *mercurytestutility.exe* and ensure the version code has been picked up in Help > About.<BR>
  Note: you may have to make a minor edit (e.g. add a space and delete it) in<BR>
  *mercurytestutility.pro* and/or *mainwindow.cpp*<BR>
  to force the compiler to pick up the new Git tag.
  
- Place the new statically linked .exe in:<BR>
  *mercurytestutility\installer\packages\com.vendor.product\data*<BR>
  (don't rename it)  
  
- Open *mercurytestutility\installer\config\config.xml* and update the version field to match the version in the new build.<BR>
  e.g.<BR>
  `<Version>1.1.0</Version>`

- Open *mercurytestutility\installer\packages\com.vendor.product\meta\package.xml*, update the version field to match the version in the new build as above
and update the `<ReleaseDate>` field.

- Open a command prompt at the *mercurytestutility\installer* directory.

- Execute the following command, replacing X.Y.Z with the new version code<BR>
  (the QtIFW installation directory and/or version may have to be modified for the installation on your PC.):
  <PRE>C:\Qt\QtIFW2.0.1\bin\binarycreator.exe -c config\config.xml -p packages MercuryTestUtilityInstaller_vX.Y.Z.exe</PRE>

<P>Note - ignore these errors:<BR>
    `qt.network.ssl: QSslSocket: cannot resolve SSLv2_client_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve TLSv1_1_client_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve TLSv1_2_client_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve SSLv2_server_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve TLSv1_1_server_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve TLSv1_2_server_method`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve SSL_select_next_proto`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve SSL_CTX_set_next_proto_select_cb`<BR>
    `qt.network.ssl: QSslSocket: cannot resolve SSL_get0_next_proto_negotiated`<BR>