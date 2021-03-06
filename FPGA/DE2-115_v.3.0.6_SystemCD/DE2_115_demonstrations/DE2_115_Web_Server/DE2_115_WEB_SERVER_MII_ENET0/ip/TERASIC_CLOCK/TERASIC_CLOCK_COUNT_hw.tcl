# TCL File Generated by Component Editor 9.1sp1
# Fri Feb 26 13:53:39 CST 2010
# DO NOT MODIFY


# +-----------------------------------
# | 
# | TERASIC_CLOCK_COUNT "TERASIC_CLOCK_COUNT" v1.0
# | null 2010.02.26.13:53:39
# | 
# | 
# | D:/svn/de2_115_golden_sopc/ip/TERASIC_CLOCK/TERASIC_CLOCK_COUNT.v
# | 
# |    ./TERASIC_CLOCK_COUNT.v syn, sim
# | 
# +-----------------------------------

# +-----------------------------------
# | request TCL package from ACDS 9.1
# | 
package require -exact sopc 9.1
# | 
# +-----------------------------------

# +-----------------------------------
# | module TERASIC_CLOCK_COUNT
# | 
set_module_property NAME TERASIC_CLOCK_COUNT
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property GROUP "Terasic Technologies Inc./DE2_115"
set_module_property DISPLAY_NAME TERASIC_CLOCK_COUNT
set_module_property TOP_LEVEL_HDL_FILE TERASIC_CLOCK_COUNT.v
set_module_property TOP_LEVEL_HDL_MODULE TERASIC_CLOCK_COUNT
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property ANALYZE_HDL TRUE
# | 
# +-----------------------------------

# +-----------------------------------
# | files
# | 
add_file TERASIC_CLOCK_COUNT.v {SYNTHESIS SIMULATION}
# | 
# +-----------------------------------

# +-----------------------------------
# | parameters
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | display items
# | 
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point slave
# | 
add_interface slave avalon end
set_interface_property slave addressAlignment NATIVE
set_interface_property slave associatedClock clock_sink
set_interface_property slave burstOnBurstBoundariesOnly false
set_interface_property slave explicitAddressSpan 0
set_interface_property slave holdTime 0
set_interface_property slave isMemoryDevice false
set_interface_property slave isNonVolatileStorage false
set_interface_property slave linewrapBursts false
set_interface_property slave maximumPendingReadTransactions 0
set_interface_property slave printableDevice false
set_interface_property slave readLatency 0
set_interface_property slave readWaitTime 1
set_interface_property slave setupTime 0
set_interface_property slave timingUnits Cycles
set_interface_property slave writeWaitTime 0

set_interface_property slave ASSOCIATED_CLOCK clock_sink
set_interface_property slave ENABLED true

add_interface_port slave s_address_in address Input 2
add_interface_port slave s_read_in read Input 1
add_interface_port slave s_readdata_out readdata Output 32
add_interface_port slave s_write_in write Input 1
add_interface_port slave s_writedata_in writedata Input 32
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point conduit_end
# | 
add_interface conduit_end conduit end

set_interface_property conduit_end ENABLED true

add_interface_port conduit_end CLK_1 export Input 1
add_interface_port conduit_end CLK_2 export Input 1
# | 
# +-----------------------------------

# +-----------------------------------
# | connection point clock_sink
# | 
add_interface clock_sink clock end

set_interface_property clock_sink ENABLED true

add_interface_port clock_sink s_clk_in clk Input 1
add_interface_port clock_sink s_reset_in reset Input 1
# | 
# +-----------------------------------
