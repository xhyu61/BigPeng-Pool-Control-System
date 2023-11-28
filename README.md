# BigPeng-Pool-Control-System

![Static Badge](https://img.shields.io/badge/Lang-C%2B%2B-pink)
![Static Badge](https://img.shields.io/badge/Lang-Python-blue)
![Static Badge](https://img.shields.io/badge/Lang-HTML-orange)

It's a homework for Harbin Engineering University Embedded System Design course, not a real project. So there may have some mistake. Getting Started below can help you run the project.

## Getting Started

**You need to install Proteus and database like SQL first.**

I finished this work on vscode, so I will introduce the method on vscode. If you have pycharm or other IDE, it's also ok.

### 1.Platform IO & main.cpp

![image](https://github.com/xhyu61/BigPeng-Pool-Control-System/assets/105192438/f9eef0d7-459a-4cfe-8af6-af959ae27491)

 You need to get an extension called Platform IO. "Proteus Files/main.cpp" should run in it. 
 
 New one project and new one .cpp file. You can copy the code into it. Build it by a "√" below the window. Download the package that you don't have until the build success.

 ![image](https://github.com/xhyu61/BigPeng-Pool-Control-System/assets/105192438/87c693da-0795-4749-8455-681d63a770b4)

 Then you'll got an .elf file. The path of your .elf file may be the same as me. Mine is 
 
 ```
 Documents\PlatformIO\Projects\BigPeng\.pio\build\uno\firmware.elf
 ```

### 2.Proteus & test1.pdsprj

Open test1.pdsprj by Proteus. Double-click the component bounded in the image below.

![image](https://github.com/xhyu61/BigPeng-Pool-Control-System/assets/105192438/6f4f0442-8c2e-456d-8d13-e4740270429a)

You will see this: 

![image](https://github.com/xhyu61/BigPeng-Pool-Control-System/assets/105192438/95acb6c8-9d4e-48b0-b371-9d10f8d3fd36)

Check if `CKSEL Fuses` is 1111, `Clock Frequency` is 16MHz. If not, set them.

Add the .elf you built just now to the `Program File`. (You should do this every time you open this file by proteus)

Click Start at lower left corner to start the simulation.

### 3.vscode & Database & 3 python files

Launch vscode, open folder which contains 3 python files: index.py, myserial.py, sqlapi.py.

Open SQL (or other database). Run sqlapi.py to build a database. You may check the function `connect_db()` if the connection is correct to you.

There has 3 tables in the database called "dapeng": users, logs, e_logs.

Then run index.py, the account can be seen in the "users" table in database.

**Don't run myserial.py!** Because it's used to open the port between the python file and proteus, and it can only open once before closed.

After logging in the page, click "实时监测" to get new data. Make sure you start simulation in proteus.

You can change the value of the sensor in proteus. Noted that the value of the sensor is not the same to the value you see in the web page. Try to change temperature, pH by DHT11 component, change NTU by LDR, change ORP by BMP180. **You need to refresh the page to see the new value. 1 update happens in 1 second.**

Check what will happened if the temperature is lower than 25℃ or higher than 27℃, or if pH is lower than 6.8 or higher than 7.2, or if the NTU is higher than 1, or if the ORP is lower than 650.


## Special Thanks

**This work is origined by an old work, and I change the topic and modify the code of files written by C++, python, HTML. If you know who is the original writer, please tell me.**

The logo of the web page is hydro-element in Genshin Impact.
