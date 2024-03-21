# 基于esp idf的数码管显示驱动
采用spi，支持74HC595的数码管
# 使用方法
1.拷贝digitron.c和digitron.h文件到你自己的项目文件夹中。(如有必要在CMakeLists.txt文件中添加digitron.c)
2.更改digitron.c文件中的SDI_PIN、CLK_PIN、LOAD_PIN三个宏定义，改成你自己的引脚。
3.调用digitron_init()，初始化后，即可调用digitron_display_int()打印数字。

