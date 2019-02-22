
echo "Warning, this script must be called from the build folder (e.g. faustdir)"
cmake -DPACK=on ..
cmake --build . --config Release --  /maxcpucount:4
cpack -G NSIS64
move GuidoLib-*.exe ..