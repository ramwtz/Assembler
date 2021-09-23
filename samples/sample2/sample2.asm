V:    add     $15,$27,$31
      sb      $15,231,$2
      sub     $3,$23,$19
      nor     $21,$23,$25
      nori    $13,324,$2
R:    lb      $4,345,$7
      lw      $30,-332,$31
      mvlo    $21,$22
      subi    $31,564,$12
      andi    $12,157,$21
      and     $2,$5,$13
STR:  .asciz  "hi how are you"
END:    stop
.entry R
      ori     $1,-234,$23
      blt     $3,$22, STR
      or      $23,$5,$30
      call    V