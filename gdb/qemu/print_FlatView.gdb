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
        printf "\tmr name:%s offset_in_region:0x%lx start:0x%llx size:0x%llx\n", $range.mr->name, $range.offset_in_region, $start[0], $size[0]
        # print $range.addr
        set $i = $i + 1
    end
end
