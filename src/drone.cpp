// #include <drone.h>
// #include <cstdlib>

// drone::drone(){
//     printf("bruhconk\n");
//     fl.set_pin(MOTOR_FL_PIN);
//     fr.set_pin(MOTOR_FR_PIN);
//     bl.set_pin(MOTOR_BL_PIN);
//     br.set_pin(MOTOR_BR_PIN);
// }

// void drone::start(){
//     if(ENABLE_FL){
//         printf("fuck this shit\n");
//         fl.start();
//     }
//     if(ENABLE_FR){
//         fr.start();
//     }
//     if(ENABLE_BL){
//         bl.start();
//     }
//     if(ENABLE_BR){
//         fr.start();
//     }
// }

// void drone::set_all(int width){
//     fl.pulse_width = fr.pulse_width = bl.pulse_width = br.pulse_width = width;
// }

// void drone::arm(){
//     // fl.pulse_width = THROTTLE_MIN;
//     set_all(THROTTLE_MIN);
//     usleep(2000000);
//     // fl.pulse_width = THROTTLE_MAX;
//     set_all(THROTTLE_MAX);
//     usleep(2000000);
//     // fl.pulse_width = THROTTLE_MIN;
//     set_all(THROTTLE_MIN);
//     usleep(1000000);
// }

// void drone::close(){
//     fl.close();
//     fr.close();
//     bl.close();
//     br.close();
// }

// void drone::set_diagonals(short diagonal, int width){
//     switch(diagonal){
//     case FLBR_DIAGONAL:
//         fl.pulse_width = br.pulse_width = width;
//         break;
//     case FRBL_DIAGONAL:
//         fr.pulse_width = bl.pulse_width = width;
//         break;
//     default: 
//         printf("Invalid diagonal: Diagonal %d : drone::set_diagonals\n", diagonal);
//     }
// }

// void drone::set_motor(short motor, int width){
//     switch(motor){
//     case MOTOR_FL:
//         fl.pulse_width = width;
//         break;
//     case MOTOR_FR:
//         fr.pulse_width = width;
//         break;
//     case MOTOR_BL:
//         bl.pulse_width = width;
//         break;
//     case MOTOR_BR:
//         br.pulse_width = width;
//         break;
//     default:
//         printf("Invalid motor: Motor %d : drone::set_motor\n", motor);
//     }
// }

// void drone::enable(bool flb, bool frb, bool blb, bool brb){
//     if(flb){
//         fl.start();
//     }else{
//         fl.stop();
//     }
//     if(frb){
//         fr.start();
//     }else{
//         fr.stop();
//     }
//     if(blb){
//         bl.start();
//     }else{
//         bl.stop();
//     }
//     if(brb){
//         br.start();
//     }else{
//         br.stop();
//     }
// }

