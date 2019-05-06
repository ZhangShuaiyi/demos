define my_print_FlatView
    set $view = $arg0
    printf "nr_allocated:%d nr:%d ref:%d\n", $view->nr_allocated, $view->nr, $view->ref
    printf "root mr:%p %s\n", $view->root, $view->root->name
    printf "ranges:\n"
    set $i = 0
    while $i < $view->nr
        # print $view->ranges[$i]
        set $range = $view->ranges[$i]
        set $start = (long long *)(&$view->ranges[$i].addr.start)
        set $size = (long long *)(&$view->ranges[$i].addr.size)
        printf "mr name:%s offset_in_region:0x%lx start:0x%llx size:0x%llx alias:%p\n", $range.mr->name, $range.offset_in_region, $start[0], $size[0], $range.mr->alias
        set $i = $i + 1
    end
end

define my_print_FlatView_detail
    set $view = $arg0
    printf "nr_allocated:%d nr:%d ref:%d\n", $view->nr_allocated, $view->nr, $view->ref
    printf "root mr:%p %s\n", $view->root, $view->root->name
    printf "ranges:\n"
    set $i = 0
    while $i < $view->nr
        # print $view->ranges[$i]
        set $range = $view->ranges[$i]
        set $start = (long long *)(&$view->ranges[$i].addr.start)
        set $size = (long long *)(&$view->ranges[$i].addr.size)
        printf "mr name:%s offset_in_region:0x%lx start:0x%llx size:0x%llx alias:%p\n", $range.mr->name, $range.offset_in_region, $start[0], $size[0], $range.mr->alias
        if $range.mr->ram
            set $mr = $range.mr
            set $ram = $range.mr->ram_block
            set $size=*(long long (*)[2])(&$mr->size)
            printf "\tmr:%s addr:0x%lx size:0x%llx ram_block:%p max_length:0x%lx\n", $mr->name, $mr->addr, $size[0], $ram, $ram->used_length
        end
        set $i = $i + 1
    end
end
