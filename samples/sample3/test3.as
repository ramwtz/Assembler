.extern V
X:      beq     $23,$10,F
C:      bne     $21,$22,MAIN
        .dw     21314,32423,5645,675
        sw      $12,345,$3
        lh      $31,3423,$3
        jmp     F
MAIN:   move    $10,$20
        mvhi    $3,$13
        bgt     $9,$8,MAIN
F:      addi    $4,-44,$23
        sh      $12,-23,$23
CNT:    jmp     $2
        la      V
W:      .db     1,34,76,24
        .dh     23,45,75,32
END:    stop
.entry  W
