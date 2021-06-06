#!/usr/bin/env python

import os
import sys
import optparse

# we need to import some python modules from the $SUMO_HOME/tools directory
if 'SUMO_HOME' in os.environ:
    tools = os.path.join(os.environ['SUMO_HOME'], 'tools')
    sys.path.append(tools)
else:
    sys.exit("please declare environment variable 'SUMO_HOME'")


from sumolib import checkBinary  # Checks for the binary in environ vars
import traci


def get_options():
    opt_parser = optparse.OptionParser()
    opt_parser.add_option("--nogui", action="store_true",
                         default=False, help="run the commandline version of sumo")
    options, args = opt_parser.parse_args()
    return options


# contains TraCI control loop
def run():
    lss = {"0":0,"1":3,"2":6,"3":9,"4":12,"ev_0":16}
    step = 0
    while traci.simulation.getMinExpectedNumber() > 0:
        traci.simulationStep()
       # print(step)

        if step == 100:
         # traci.route.add("trip", ["e1", "e4"])
          traci.vehicle.add("7", "route_0", typeID="ev")
          lss["7"] = step
        
        if step%10:
            #ls = traci.domain.Domain.getIDList()
            #print(ls)

            #ls = traci.domain.getIDList()
            print("step : ",step)
            for vehi in lss:
                if lss[vehi]<step:
                    print(vehi,traci.vehicle.getPosition(vehi))
        # if step == 150:
        #   position = traci.vehicle.getPosition('7')
        #   print(position)
        step += 1
    
    traci.close()
    sys.stdout.flush()


# main entry point
if __name__ == "__main__":
    options = get_options()

    # check binary
    if options.nogui:
        sumoBinary = checkBinary('sumo')
    else:
        sumoBinary = checkBinary('sumo-gui')

    # traci starts sumo as a subprocess and then this script connects and runs
    traci.start([sumoBinary, "-c", "demo.sumocfg",
                             "--tripinfo-output", "tripinfo.xml"])
    run()