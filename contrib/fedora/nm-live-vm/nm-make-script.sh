#!/bin/bash

set -vx

die() {
    echo "$@" >&2
    exit 1
}

COMMIT=${1:-origin/master}

URL="${2:-"git://anongit.freedesktop.org/NetworkManager/NetworkManager"}"

passwd -d root
test -d /NetworkManager || (
    git init /NetworkManager
    cd /NetworkManager

    # check if there is a local git repository and fetch from it first (should be faster)
    test -d "/NetworkManager-local.git" && (
        git remote add local "/NetworkManager-local.git"
        git fetch local
        git remote remove local
        rm -rf "/NetworkManager-local.git"
    )
    git remote add origin "$URL"
)
cd /NetworkManager/ || exit 1
git fetch origin || die "Could not fetch $URL"
git checkout -f "$COMMIT" || exit 1
git clean -fdx
export CFLAGS='-g -Og'
export CXXFLAGS='-g -Og'
./autogen.sh --prefix=/usr \
             --exec-prefix=/usr \
             --libdir=/usr/lib \
             --sysconfdir=/etc \
             --localstatedir=/var \
             --with-nmtui=yes \
             --enable-gtk-doc || exit 1
make || exit 1
#make check || exit 1
make install || exit 1
cat <<EOF > /etc/NetworkManager/NetworkManager.conf
[main]
plugins=ifcfg-rh
debug=RLIMIT_CORE
[logging]
level=DEBUG
domains=ALL
EOF
/bin/systemctl enable NetworkManager.service || exit 1
/bin/systemctl enable sshd.service || exit 1

git config --global user.name "NetworkManager Test VM"
git config --global user.email "networkmanager-maint@gnome.bugs"

cat <<EOF > /root/.vimrc
EOF

cat <<EOF > /root/.bashrc
LS_OPTIONS='--color=auto -Q'
alias l='ls \$LS_OPTIONS -la'
alias ll='ls \$LS_OPTIONS -l'
EOF

# allow login for root via SSH, without password!!
sed -e 's/^#\?\(PermitRootLogin *\).*/\1yes/' \
    -e 's/^#\?\(PermitEmptyPasswords *\).*/\1yes/' \
    -i /etc/ssh/sshd_config

# disable rate limiting of the journal
sed -e 's/^#\?\(RateLimitInterval *= *\).*/\10/' \
    -e 's/^#\?\(RateLimitBurst *= *\).*/\10/' \
    -i /etc/systemd/journald.conf

mkdir /mnt/sda1

git gc
