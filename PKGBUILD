# Maintainer's Red or something, can't even care for AUR.

pkgname=depfetch
pkgver=1.0
pkgrel=0
pkgdesc="A simple program that does something"
arch=('i686' 'x86_64')
license=('GPL')
url='https://github.com/RSKYS/depfetch'
depends=()
makedepends=('pacman' 'gcc' 'paru-bin')
optdepends=()
conflicts=()
provides=()
source=('main.c')

package() {
    mkdir -p "$pkgdir/usr/bin"
    gcc -o "$pkgdir/usr/bin/depfetch" "${srcdir}/main.c"
}

md5sums=('SKIP')
