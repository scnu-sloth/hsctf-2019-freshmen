cp ./1.jpeg ./jichou.jpeg
cat ./3.bmp >> ./jichou.jpeg


# binwalk 可以看到 0x3525 分割了有其他data
# 手动把0x3525之前的删掉会得到另一张图片(bmp)
# stegsolve双图add拿flag
