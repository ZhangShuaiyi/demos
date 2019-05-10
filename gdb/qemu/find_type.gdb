define my_get_type
    set $table = type_table_get::type_table
    set $i = 0
    while $i < $table->size
        set $k = $table->keys[$i]
        if $k && $_streq((char *)$k, $arg0)
            print (Type)$table->values[$i]
            set $arg1 = (Type)$table->values[$i]
            loop_break
        end
        set $i = $i + 1
    end
end

define my_print_init_type_list_qom
    set $l = init_type_list[2]
    set $e = $l.tqh_first
    set $i = 0
    while $e
        print $e->init
        set $e = $e->node.tqe_next
        set $i = $i + 1
    end
    printf "total type init:%d\n", $i
end
