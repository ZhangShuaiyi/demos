define my_print_ram_list
    set $blk = ram_list.blocks->lh_first
    while $blk
        printf "RAMBlock 0x%lx {mr:0x%lx(%s), offset:0x%lx, used_length:0x%lx, max_length:0x%lx idstr:%s fd:%d}\n", \
            $blk, $blk->mr, $blk->mr->name, $blk->offset, $blk->used_length, $blk->max_length, $blk->idstr, $blk->fd
        set $blk = $blk->next.le_next
    end
end
