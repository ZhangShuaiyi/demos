define my_print_qom
    set $obj = (Object *)$arg0
    print $obj->class->type->name
    set $prop = $obj->properties
    set $i = 0
    while $i < $prop->size
        set $k = $prop->keys[$i]
        if $k
            set $v = (ObjectProperty *)$prop->values[$i]
            print {(char *)$k, $v->name, $v->type, $v->opaque}
        end
    set $i = $i + 1
    end
end

define my_print_root_qom
    my_print_qom object_get_root::root
end

define my_print_machine
    my_print_qom qdev_get_machine::dev
end

define my_object_get
    set $obj = (Object *)$arg0
    set $prop = $obj->properties
    set $i = 0
    while $i < $prop->size
        set $k = $prop->keys[$i]
        if $k && $_streq((char *)$k, $arg1)
            set $t = (ObjectProperty *)$prop->values[$i]
            print $t
            if $_regex($t->type, "child<.*>")
                set $arg2 = (Object *)$t->opaque
            else
                printf "type is (%s) $arg2 is NULL\n", $t->type
            end
            loop_break
        end
        set $i = $i + 1
    end
end
