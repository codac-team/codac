%
% Known issues:
% It might not be possible to view the VIBes windows, possibly use the VIBes functions to save the desired pictures as a workaround...
%
% TODO:
% Determine automatically the Python version used by MATLAB Online (e.g. by analyzing pyenv output) and download the latest codac and vibes versions from PyPi.
% Replace "/MATLAB Drive" with the output of pwd (warning: might be different than $HOME...).
% Add hint at the end to indicate how to uninstall (e.g. remove pythonlibs folder...)...
%

!lsb_release -a
!uname -a
pyenv

disp('Downloading and installing prerequisites...');

% pip install --user codac4matlab
!mkdir -p "/MATLAB Drive/pythonlibs"
if ~isfolder("/MATLAB Drive/pythonlibs/codac4matlab")
    % For another version, get the exact link and name from https://pypi.org/project/codac4matlab/#files
    !cd "/MATLAB Drive/pythonlibs" ; rm -f codac4matlab-2.1.0-cp313-cp313-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl
    !cd "/MATLAB Drive/pythonlibs" ; wget -q --show-progress --progress=dot:giga https://files.pythonhosted.org/packages/cd/8f/260a85ecd832120922510f6952d264c3c0aaa279741bffa98bea67baf07b/codac4matlab-2.1.0-cp313-cp313-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl
    !cd "/MATLAB Drive/pythonlibs" ; unzip -q codac4matlab-2.1.0-cp313-cp313-manylinux_2_27_x86_64.manylinux_2_28_x86_64.whl
end
if ~isfolder("/MATLAB Drive/pythonlibs/vibes-0.2.2")
    % For another version, get the exact link and name from https://pypi.org/project/vibes/#files
    !cd "/MATLAB Drive/pythonlibs" ; rm -f vibes-0.2.2.tar.gz
    !cd "/MATLAB Drive/pythonlibs" ; wget -q --show-progress --progress=dot:giga https://files.pythonhosted.org/packages/20/3a/c19179d02310793aae928682a6435f97fe57e892f057048f1624b3675474/vibes-0.2.2.tar.gz
    !cd "/MATLAB Drive/pythonlibs" ; tar xfz vibes-0.2.2.tar.gz
end

% sudo apt-get install -y vibes
if ~isfolder("/MATLAB Drive/vibes")
    !mkdir -p "/MATLAB Drive/vibes"
    !cd "/MATLAB Drive/vibes" ; rm -f VIBes-viewer_amd64.AppImage
    !cd "/MATLAB Drive/vibes" ; wget -q --show-progress --progress=dot:giga https://github.com/ENSTABretagneRobotics/VIBES/releases/latest/download/VIBes-viewer_amd64.AppImage
    !cd "/MATLAB Drive/vibes" ; ./VIBes-viewer_amd64.AppImage --appimage-extract >/dev/null
    !cd "/MATLAB Drive/vibes" ; mv squashfs-root/lib/x86_64-linux-gnu/libc.so.6 libc.so.6.bak
    % To run in the background, but probably no way to make the window might visible...
    !QT_QPA_PLATFORM_PLUGIN_PATH="/MATLAB Drive/vibes/squashfs-root/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/" ./squashfs-root/usr/bin/VIBes-viewer &
    !xwininfo -root -tree | grep VIBes
    % Maybe capture it with [~,out] = system('xwininfo -root -tree | grep "Codac - default figure"'); wid = regexp(out,'0x[0-9A-Fa-f]+','match','once'); system(sprintf('xwd -id %s -silent -out vibes.xwd', wid));...
end

disp('End of prerequisites installation...');

% This is probably necessary every time...
if count(py.sys.path, "/MATLAB Drive/pythonlibs") == 0
    insert(py.sys.path, int32(0), "/MATLAB Drive/pythonlibs");
end
