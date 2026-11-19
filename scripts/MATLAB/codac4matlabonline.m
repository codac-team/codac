%
% Since pip and AppImage do not seem to work on MATLAB Online, this script downloads and extracts codac4matlab and VIBes in the "/MATLAB Drive" folder.
%
% Known issues:
% It might not be possible to view the VIBes windows, possibly use the VIBes functions to save the desired pictures as a workaround...
%
% TODO:
% Replace "/MATLAB Drive" with the output of matlabdrive command (warning: might be different than $HOME...)? Might not work with shell commands prefixed by "!", but maybe with e.g. drive = matlabdrive; system('wget ... "' + drive + '/..."')...?
%

!lsb_release -a
!uname -a
pyenv

disp('Checking, downloading and installing codac4matlab prerequisites in "/MATLAB Drive/pythonlibs" and "/MATLAB Drive/vibes"...');

% Equivalent of pip install --user codac4matlab
!mkdir -p "/MATLAB Drive/pythonlibs"
if ~isfolder("/MATLAB Drive/pythonlibs/codac4matlab")
    % If it does not work, please download the necessary .whl from https://pypi.org/project/codac4matlab/#files and unzip it in "/MATLAB Drive/pythonlibs"...
    !PY_V_MAJ=`python3 -c "import sys; print(sys.version_info[0])"`; PY_V_MIN=`python3 -c "import sys; print(sys.version_info[1])"`; CP="cp${PY_V_MAJ}${PY_V_MIN}"; VER=$(wget -qO- https://pypi.org/pypi/codac4matlab/json | grep -o '"version":"[^"]*' | head -n 1 | cut -d'"' -f4); URL=$(wget -qO- https://pypi.org/pypi/codac4matlab/json | grep -o '"url":"[^"]*' | cut -d'"' -f4 | grep "/codac4matlab-${VER}-${CP}-${CP}.*manylinux.*x86_64\.whl$" | head -n 1); cd "/MATLAB Drive/pythonlibs" && wget -q -O "$(basename "$URL")" --show-progress --progress=dot:giga "$URL" && unzip -q "$(basename "$URL")"
end
d = dir("/MATLAB Drive/pythonlibs/vibes-*");
if ~any([d.isdir])
    % If it does not work, please download the necessary .tar.gz or .whl from https://pypi.org/project/vibes/#files and extract it in "/MATLAB Drive/pythonlibs"...
    !VER=$(wget -qO- https://pypi.org/pypi/vibes/json | grep -o '"version":"[^"]*' | head -n 1 | cut -d'"' -f4); URL=$(wget -qO- https://pypi.org/pypi/vibes/json | grep -o '"url":"[^"]*' | cut -d'"' -f4 | grep "/vibes-${VER}\.tar\.gz$" | head -n 1); cd "/MATLAB Drive/pythonlibs" && wget -q -O "$(basename "$URL")" --show-progress --progress=dot:giga "$URL" && tar xfz "$(basename "$URL")"
end

% VIBes-viewer
if ~isfolder("/MATLAB Drive/vibes/squashfs-root")
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

disp('End of codac4matlab prerequisites checks.');

% This is probably necessary every time...
if count(py.sys.path, "/MATLAB Drive/pythonlibs") == 0
    insert(py.sys.path, int32(0), "/MATLAB Drive/pythonlibs");
end

disp('Notes:');
disp('_ This script needs to be run every time MATLAB Online is started.');
disp('_ To uninstall, remove the "/MATLAB Drive/pythonlibs" and "/MATLAB Drive/vibes" folders, and remove related entries from py.sys.path.');
