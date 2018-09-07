# skynet_v3

Control code for the Heart Lake 296B Vex Robotics team. The system is designed around a main file, and several subsystems included as headers.
Each subsystem has it's own update function and data struct which is called and maintained by the main file.

Of special interest is the generic PID system that can be plugged into any system that can calculate an error and takes numeric output.
