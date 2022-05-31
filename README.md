
# Battery Balancing Algorithm and simulator

## 1. REQUIREMENTS

The requirement is to design a battery balancing algorithm for a battery pack for the discharge cycle and implement a simulator for testing the algorithm.
The application should

 1. Implement the battery balancing algorithm such that
		 1. No cell discharge into another
		 2. Battery cuts off if voltage drops below a limit
2. Implement the simulator with API to 
		1. set initial voltages of the cells
		2. set series and load resistances
		3. get output voltage and current
		4. get current sourced by each cell
		5. get state of the switches
3. Be runnable in Linux

## 2. DESIGN

The system requirements implies that there should be simulator and a battery pack present in the system. Also there should be a driver function to  run the simulator. So the system is divided into three parts:

 - The Battery Pack
 - The Simulator
 - The driver program

### 2.1 The Battery Pack

The battery pack is the main component of the system. A battery pack consists of one or more cells and equal number of switches. A battery pack can be connected with a load and then it will run the circuit and update the cells and switches status.
#### 2.1.1 Cell
The cell is the fundamental element of a battery pack. A cell consists of a voltage source and an internal series resistance. The cell has its own characteristics like discharge curve, capacity and voltage.
### 2.2 The Simulator

The simulator is a component that binds the battery pack with load and provides API to start, stop, pause and resume simulation of the battery.

Start and resume API resembles connecting the battery with a load making a closed circuit. Pause resembles disconnecting the load and making open circuit, where stop internally pauses and resets the battery.
### 2.3 The driver program

The driver program creates the environment to run the simulator. This program implements a command line interpreter that will take user input and perform necessary action. This program creates the cells and battery. connects them to a simulator, and passes user commands to simulator. 

## 3. IMPLEMENTATION

The implementation of the above design consists of four classes, namely, Cell, Battery, Simulator and Command Parser. Cell and Battery are the two most important components that actually implements the balancing algorithm and battery pack. The simulator and the command parser are used to demonstrate the battery pack in a Linux environment.
### 3.1 Cell

The cell is a voltage source with an internal series resistance. The voltage source has its characteristics discharge curve and capacity.

The cell provides method to set initial voltage, series resistance, capacity and four other characteristics parameter discussed later. It also provides APIs to get sourced current and cell voltage at any time. The cell can update itself when serving a particular current for a specific duration.

The initial voltage, capacity and the discharge curve together defines the cell’s characteristics. Capacity of the cell is measured in mAh and it signifies how long the cell can provide power. The discharge curve shows the relation between the output voltage of the cell with respect to its current capacity. The discharge curve is elaborated below.
#### 3.1.1 The discharge curve

The discharge curve is the voltage vs capacity curve that determines the output voltage of the cell for its current capacity. A typical lithium-ion cells discharge curve looks like the blue curve.
For simplicity of calculation, the implementation considers a three step linear approximation of the discharge curve. So, it looks like the red lines.

![dc.png](https://lh4.googleusercontent.com/xpu4Tzay3H4jCt1-U8zYMxF9Nn3xIXVYSoTtC3OMYzenXd7Z02VyT368_BeYr0kKTwlAl6K5F12WYXS9L8sPO8gJWDJj-Lvnyh2rJvEnN9ZmP1muuvjpkxqtUcIPZqBV4xuWHgoO53x4jSDk)

This discharge curve incorporates four new quantities, namely, shift1, shift2, drop1, and drop2. Shift1 and shift2 are two gradient shifting points on the capacity axis. These quantities are expressed as percentage of capacity discharged for convenient. Drop1 and drop2 are percentage drop of voltages at shift1 and shift2 respectively. With these four quantity, the gradients can easily be calculated and the discharge curve can be approximated using three linear equations. 

Let the three gradients are m1, m2 and m3, then

 ![](https://lh6.googleusercontent.com/wjCWZrEl7oGgYEIFJiq-sOt64d9wkN63bHNcwFPfZVARKaJGEtKINrc876Ouu9r01ykcRbhLVuRh8DpsxUVvzJUO-E_qPCZ1qjzPhyfnb1G-GehHGp8KGRBNu-hBXMYZyFkWEJOHNC2MvBoI),
 
 ![](https://lh4.googleusercontent.com/1Fe7HL1XqkqOz0BlCCxyODF5B4yRJxKIBP53Lnpo7hHIAD_Y8ISUDHMj028slcpPy1kB0jli9Rxb0SVkoZLltuS3HTHzkIOQGQXmmu2zBGz3ugtawil-YF6WsHvGC-SbrfpKYFZ8JymIfIpB)
 
 and
 
![](https://lh3.googleusercontent.com/0r1_oeKD-aVdNfT0AHcuDWtqm5Kp0pygMB7sD30crnaqymOlSV0nh7oAXdEobbzgYK1N15Ws4hZdNiNp8POoVnhMd_RO_Mb-rR8rJaCvd_pV418rXrVsDkc-KwPrjZgRRDhsJ1nAqStEFU4j)

and the straight line equation is

![](https://lh6.googleusercontent.com/R6I9L350HX6HAF1-qRHht1N_J28cj2DBZO_EgdhcvKogC2VUwd0Zi93m7y1RdWReEXcRRA4aEyIM_jZ8gKbl-E5ZaH_DUZGLhmGB3dXLbnGXmNOajhgV4I6U6pB1PhLGaYdBHPrzHC8W5sf0)

where Cd is discharged capacity. This equation is used to calculate the voltage of the cell at any point.

### 3.2 Battery

The Battery resembles a battery pack with one or more cells. This implementation assumes three cells are present in a battery pack. Other than the cells, the battery pack has switches for each cell to connect or disconnect that cell.The battery provides a output voltage and when connected to a load also the output current.

The design assumes that the output voltage is the voltage of the connected cell that has minimum cell voltage. It also assumes that the internal series resistance are negligible compared to the connected load. 

The source current of the connected cells are calculated by dividing the output current in a ratio that is directly proportional to the potential difference and inversely proportional to the internal series resistance.

The battery pack provides APIs to add cells to battery, set cutoff voltages, get switch states, output voltage and current, and run, stop and reset the battery.

The battery actually implements the balancing algorithm by operating the switches when the battery is connected to a load and running, i.e. closed circuit.
### 3.3 Simulator

The Simulator connects the Battery pack and the load. And it provides various APIs to operate the battery. It starts, stops, pauses and resumes the simulation.
These operations are actually wrapper to the battery APIs. This give the user more option and flexibility to test the battery.
### 3.4 Command Parser

The command parser is a very important part of the application as it takes user input and gives the command to execute. The command parser takes a valid set of commands and sub commands. It provides a command line prompt for user input.

When user gives any command, the parser matches them with the valid command set and returns the function number if there is a match. According to the function number appropriate actions can be taken. This is done by the Driver program.
### 3.5 Driver program

The driver program prepares the environment for testing the simulator. It creates cells, battery, simulator, connects them and provides a command line interface for the user via the command parser to process user input.

The driver program is responsible for taking user input, processing it, taking appropriate actions and formatting the output to present to the user back.
## 4. USAGE

Using this application requires building it in a Linux machine and running. The battery and cell classes can also be used in other programs.
### 4.1 Building

To build the application, Open a terminal in Linux and change directory to the base directory of the application.
cd _<path/to/batsim>_

Then use make clean to delete any previous temporary files and binaries present.(Optional)
`make clean`
Then simply run `make` to build the application. The executable named _batsim_ will be created.
### 4.2 Running

To run the application, Open a terminal in Linux and change directory to the base directory of the application.
cd <path/to/gp_batsim>
Then enter the executable name as follows
./batsim
The application will provide with a prompt like batsim>>
#### 4.2.1 Commands and Keywords

The application currently supports 6 commands and 16 keywords. The following list describes them in details.
- Commands:
`get`, `set`, `setadv`, `sim`, `help`, `exit`
- Keywords
`cvoltage`, `load`, `scurrent`, `vout`, `iout`, `status`, `runtime`, `sresistance`, `start`, `stop`, `shift`, `drop`, `cutoff`, `capacity`, `initv`, `sstate`

1. `get`:	This command is used to get various parameters of the cells, battery or the simulator. Valid keywords for get command are `cvoltage`, `scurrent`, `vout`, `iout`, `load`, `runtime`, `sstate`, and `status`. 
The syntax is `get <keyword>`.  Any extra parameters are omitted.

2. `set`:	This command is used to set various parameters of the sells, battery and simulator. Valid keywords are `initv`, `sresistance`, and `load`. Each of this keywords have different syntax as follows:

       set initv <v1> [<v2> [<v3>]] #v1, v2 and v3 are initial voltages to be set for cell1, cell2 and cell3 respectively in Volts.

	   set load <load resistance> #load resistance is the load to be set in Ohms

	   set sresistance <r1> [<r2> [<r3>]] #r1, r2 and r3 are internal series resistance to be set for cell1, cell2 and cell3 respectively in Ohms. 
Any extra parameters are omitted for all cases. There is no check on the values user sets. Inappropriate values may lead to undefined behaviour.

3. `setadv`: This command is used to set advanced parameters for cells and Battery pack. Valid keywords are `shift`, `drop`, `cutoff` and `capacity`.The syntaxes are as follows:

	   setadv shift <cell number> <shift1> <shift2> #cell number is the serial number of the cell to be modified. Currently it is among 1 to 3. shift1 and shift2 are the two gradient shifting points expressed in percentage.

	   setadv drop <cell number> <drop1> <drop2> #cell number is the serial number of the cell to be modified. Currently it is among 1 to 3. drop1 and drop2 are the two voltage drops expressed in percentage.

	   setadv cutoff <voltage> #voltage is the cutoff voltage to be set for the battery in Volts.

	   setadv capacity <c1> [<c2> [<c3>]] #c1, c2 and c3 are capacities to be set for cell1, cell2 and cell3 respectively in mAH.

4. `sim`:	This command is used to control the simulator. Valid keywords are `start` and `stop`. Syntax is `sim <start/stop>`. Extra parameters are omitted.

5. `help`: Displays a help text. No keywords are accepted.

6. `exit`: Exits the simulator. No keywords are accepted.

Many other keywords can be supported in the current design. Those enhancements will be added in future versions.
