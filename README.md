# kprobe_recycle
rm指令文件备份
功能：
   运行rm指令时备份被删除的文件到/var/bak_dir，
   
   
Changlog:20211015

当前实现：
   运行rm 删除当前目录下文件时，可成功备份，如：
   
   #echo "akjfdkasjfdkajkdjaskjfdk" > test1
   #ls
    test1
   #rm test1
   #ls
   #ls /var/bak_bak
    test1
    完成备份
    
待处理问题：
  1：/var/bak_dir  目录下的文件通过rm指令删除时，不进行备份；（这个有实现经验，只需要后续编码处理）；
  2：rm指令后所指向的文件采用相对路径，绝对路径时问题处理，当前会出现找不到源文件的情况，后续需要编码实现；
  
总结：经过初步测试，这种方式是合适的实现方案，后期需要进一步完善就好了。
      经验总结：内核中用户空间的使用（brk);
