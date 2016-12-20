
1)
Compile and link this example:

make all

2)
And then type for example this:

./e3 -ac Hello world --logfile=MyLog -4711 "How are you?" -i42

You'll see the following output:

Non option argument in ppArgv[2]: "Hello"
Non option argument in ppArgv[3]: "world"
Non option argument in ppArgv[5]: "-4711"
Non option argument in ppArgv[6]: "How are you?"
Logfile will used: MyLog
Flags:
Flag a is set
Flag b is not set
Flag c is set
Integer value = 42

3)
Good luck! :-)

