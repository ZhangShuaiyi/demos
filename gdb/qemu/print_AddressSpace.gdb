define my_print_AddressSpace_link
    set $as=&$arg0
    while $as
        printf "%s root:%s current_map:%p\n", $as->name, $as->root->name, $as->current_map
        set $as=$as->address_spaces_link.tqe_next
    end
end
