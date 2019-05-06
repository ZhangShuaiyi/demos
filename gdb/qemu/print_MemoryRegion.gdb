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

def my_print_MemoryRegion_subregions_ops
    set $sub=$arg0->subregions.tqh_first
    while $sub
        if $sub->alias
            set $alias=$sub->alias
            print {$sub->name, $sub->ops, $alias->name, $alias->ops}
        else
            print {$sub->name, $sub->ops}
        end
        set $sub=$sub->subregions_link.tqe_next
    end
end

def my_print_MemoryRegion_subregions_alias
    set $sub=$arg0->subregions.tqh_first
    while $sub
        set $size=(long long *)(&$sub->size)
        printf "%s addr:0x%lx alias:%p alias_offset:0x%lx size(0x%llx:0x%llx)\n", $sub->name, $sub->addr, $sub->alias, $sub->alias_offset, $size[1], $size[0]
        if $sub->alias
            set $mr=$sub->alias
            set $size=*(long long (*)[2])(&$mr->size)
            printf "\talias mr:%s addr:0x%lx size:0x%llx ram:%d\n", $mr->name, $mr->addr, $size[0], $mr->ram
            if $sub->alias->ram
                set $ram=$sub->alias->ram_block
                printf "\tram_block:%p used_length:0x%lx max_length:0x%lx offset:0x%lx host:%p\n", $ram, $ram->used_length, $ram->max_length, $ram->offset, $ram->host
            end
        end
        set $sub=$sub->subregions_link.tqe_next
    end
end
