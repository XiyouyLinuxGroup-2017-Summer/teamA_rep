# emmmmmmmmmm

这种情况是因为你的e-mail地址配置不正确导致的~
在Github中选择Profile，左侧选E-mail，添加你在git config中用的邮箱，假设就叫a@b.c吧
然后在命令行中执行
git config --global user.email a@b.c
就可以了～

如果还不行的话尝试着使用
git config --global user.name xxx
把xxx替换为你的github账号名字吧～
