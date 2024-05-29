#!/bin/bash

export LATEST_TAG=`curl -IkLs -o /dev/null -w %{url_effective} "https://github.com/codac-team/codac/releases/latest" | sed 's#.*tag/\(.*\).*#\1#'`
export LATEST_VER="${LATEST_TAG:1}"

export LK_VER=`cat lk_codac_whl.ver`

echo Latest: $LATEST_VER
echo Last known: $LK_VER

if [[ -z "$LK_VER" || $(printf "%s\n%s" "$LATEST_VER" "$LK_VER" | sort -V | head -n 1) != "$LATEST_VER" && "$LATEST_VER" != "$LK_VER" ]]; then
	echo $LATEST_VER is newer than $LK_VER
	echo $LATEST_VER>lk_codac_whl.ver && cat lk_codac_whl.ver | ./gencodacwhl.sh
fi
