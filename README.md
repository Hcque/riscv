# Riscv
My design of  riscv simulator. It supports serial and 5 stage pipeline.
For running: > ./dotest.sh

Implementing process:

1)first translate binary file to operations. Make the serial execution run properly. See serial() function in RISCV.hpp for details. 
My achievements here is to be familiarity of basic context, I always print what happens in my register and parse process to make sure it heads to the right line. 

2)implenting the basic pipeline, including forwarding and stall for dealing with data harzard and control harzard. 
since this is just simulator, we just run execution of 5 stage back to front( WB, MA, EX, IF, ID), then pass instruction to their next stage. This is fine for simulting purpose.

Specifically:
for DATA HARZARD:

1)EX/MA result could be pass to the oprands before EX stage.

2)MA/WB results could be pass to "before EX" oprands.

REMARK1. One caution: add add add to the same register, we should forward the most recent result. That is the EX/WB is priority than MA/WB. Please refer to "Computer Organization and Design, Fifth Edition_ The Hardware_Software Interface". 

REMARK2. There is one special case must be stalled, that is :
lw , addi/…, because lw where get the result after MA, it could be impossible to send to addi before EX.

For code , refer to forward.hpp . Here I forward after instruction pass to the next stage. The priority of EX is by ordering last of farwading EX results, refering to parallel() in RISCV.hpp.

for CONTROLL HARZARD,
The simplest scheme is always not taking and stall one cycle. Notice I have handle almost all stall in RISCV.hpp. (see parallel())
In my code, the stall always happens after ID execution, it deal both data stall and control stall.
Here please notice:

notice1. when stall pc (program count) will not change (always address of last instruction + 4), which is actual "not taken branch".

notice2. after one cycle, it will be unstalled at the same place. 

notice3. stall 

	*kill the instruction of IF(very important), 
	
	*for data stall NOT kill instruction of ID (it will pass to EX), instead I use a lock. (see InstructionDecode.hpp)
	
The above design make me fell difficult of this project. I debuged line by line, which is sometimes frustrating.

3)real ineresting parts : branch prediction Here I implenting the two bit sature prediction and also adaptive training in "Two-Level Adaptive Training Branch Prediction".
The best is adaptive training :![image](https://user-images.githubusercontent.com/43036142/149615779-c5f4d3d1-e190-4db1-826f-72788640aece.png)

I reflect the whole design and throught. I think the main goal is to make it fast by instruction parallel, a counterpart of threads parallelism. Which will be my next project's focused point.

I also learned that to propose an idea, such as pipeline, you will also confront with problems brought to the desk, the engineer should deal with these problems, such as hazard in this project. This strategy is wroth to notice. 



========================================
