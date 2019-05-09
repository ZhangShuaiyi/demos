define my_print_pcms_bus_devices
    set $pcms = (PCMachineState *)current_machine
    set $bus = $pcms->bus
    set $i = 0
    while $i < 256
        set $dev = $bus->devices[$i]
        if $dev
            printf "====%d device 0x%lx name:%s type:%s id:", $i, $dev, $dev->name, ((Object *)(&$dev->qdev))->class->type->name
            if $dev->qdev.id
                printf "%s", $dev->qdev.id
            end
            printf "\n"
            set $n = 0
            while $n < 7
                set $r = $dev->io_regions[$n]
                if $r.size
                    printf "\t%d type:%d size:0x%lx addr:0x%lx address_space:%s memory:{%s addr:0x%lx}\n", \
                        $n, $r.type, $r.size, $r.addr, $r.address_space->name, $r.memory->name, $r.memory->addr
                end
                set $n = $n + 1
            end
        end
        set $i = $i + 1
    end
end
