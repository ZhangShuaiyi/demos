def my_print_MemoryRegion
    set $mr=$arg0
    printf "%s addr:0x%lx priority:%d ram:%d\n", $mr->name, $mr->addr, $mr->priority, $mr->ram
end

def my_print_MemoryRegion_subregions
    set $sub=$arg0->subregions.tqh_first
    while $sub
        my_print_MemoryRegion $sub
        set $sub=$sub->subregions_link.tqe_next
    end
end

def my_print_MemoryRegion_subregions_alias
    set $sub=$arg0->subregions.tqh_first
    while $sub
        set $size=*(long long (*)[2])(&$sub->size)
        printf "%s addr:0x%lx alias:%p alias_offset:0x%lx size:%llx\n", $sub->name, $sub->addr, $sub->alias, $sub->alias_offset, $size[0]
        if $sub->alias
            set $mr=$sub->alias
            if $sub->alias->ram
                set $ram=$sub->alias->ram_block
                printf "\t%p used_length:0x%lx max_length:0x%lx offset:0x%lx host:%p\n", $ram, $ram->used_length, $ram->max_length, $ram->offset, $ram->host
            end
        end
        set $sub=$sub->subregions_link.tqe_next
    end
end
