# Maintainer's Red or something, can't even care for AUR.

pkgname=depfetch
pkgver=prime
pkgrel=0
pkgdesc='Time to slay all deps.'
arch=(x86_64)
license=(GPL-3.0-with-GCC-exception)
url='https://github.com/RSKYS/depfetch'
depends=('pacman' 'paru-bin')
makedepends=('gcc')
source=('main.c')

package() {
    mkdir -p $pkgdir/usr/bin
    gcc -o "$pkgdir/usr/bin/$pkgname" "$srcdir/main.c"
}

options=('!debug' 'strip')
md5sums=('SKIP')
