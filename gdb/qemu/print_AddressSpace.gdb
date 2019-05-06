define my_print_AddressSpace_link
    set $as=&$arg0
    while $as
        printf "%s root:%s current_map:%p\n", $as->name, $as->root->name, $as->current_map
        set $as=$as->address_spaces_link.tqe_next
    end
end

define my_print_AddressSpace_listeners
    set $lst = $arg0.listeners.tqh_first
    while $lst
        print {$lst->begin, $lst->commit, $lst->region_add, $lst->address_space}
        set $lst = $lst->link_as->tqe_next
    end
end

define my_print_AddressSpace_link_listeners
    set $as=&$arg0
    while $as
        set $lst = $as->listeners.tqh_first
        printf "%s root:%s current_map:%p listeners:0x%lx\n", $as->name, $as->root->name, $as->current_map, $lst
        while $lst
            print {$lst->begin, $lst->commit, $lst->region_add, $lst->address_space}
            set $lst = $lst->link_as->tqe_next
        end
        set $as=$as->address_spaces_link.tqe_next
    end
end
