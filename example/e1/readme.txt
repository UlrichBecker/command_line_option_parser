
1)
Compile and link this example:

make all

2)
And then type for example this:

./e1 -aci42 --logfile=MyLog -4711 Hello world "How are you?" 

You'll see the following output:

Non option argument 1 in ppArgv[3]: "-4711"
Non option argument 2 in ppArgv[4]: "Hello"
Non option argument 3 in ppArgv[5]: "world"
Non option argument 4 in ppArgv[6]: "How are you?"
Logfile will used: MyLog
Flags:
Flag a is set
Flag b is not set
Flag c is set
Integer value = 42

3)
Good luck! :-)

