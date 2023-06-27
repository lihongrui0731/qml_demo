#sysroot_path=/opt/sysroot/firefly-arm64-sysroot-18.04
sysroot_path=/opt/sysroot/firefly-3399-20210525

cp libarmadillo.so.10 ${sysroot_path}/usr/lib/
cd ${sysroot_path}/usr/lib/
ln -s libarmadillo.so.10 libarmadillo.so
