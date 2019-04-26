define my_print_MemoryRegion_subregions_link_name
    set $sub=$arg0
    while $sub
        print $sub->name
        set $sub=$sub->subregions_link.tqe_next
    end
end

def my_print_MemoryRegion_subregions_name
    set $sub=$arg0->subregions.tqh_first
    my_print_MemoryRegion_subregions_link_name $sub
end
