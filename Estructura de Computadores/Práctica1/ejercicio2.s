.data
  	.align 2 #siguiente dato alineado a 4
  	A: .float 0.1, 0.2, 0.3
 	 .float 0.4, 0.5, 0.6
	
    B: .zero 24


.text
main:
	la a0 A
    la a1 B
    
    li a2 2
    li a3 3
    
	jal ra SinMatrix
    
    j fin_programa


factorial:
    mv t0 a0
	li t1 1
        
	factorial_b: beq t0 zero factorial_f
		mul t1 t1 t0
		addi t0 t0 -1
		j factorial_b
    
	factorial_f:	
    mv a0 t1
    jr ra
    

potencia: # a0 = exponente, fa0 = base
    li t0 1 # cargo un 1 (iterar)
    fmv.s ft0 fa0 # guardar la base original en ft0
    
    li t1 0
    beq a0 t1 potencia_if

	potencia_while: bge t0 a0 potencia_f
		fmul.s fa0 fa0 ft0
		addi t0 t0 1
		j potencia_while
    
    potencia_f: jr ra
    
    potencia_if:
    	li t0 1
        fcvt.s.w fa0 t0
      	jr ra

    
sin:
    
    addi sp sp -36
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    fsw fs0 12(sp)
    fsw fs1 16(sp)
    fsw fs2 20(sp)
    fsw fs3 24(sp)
    fsw fs4 28(sp)
    fsw fs5 32(sp) 
    
    li s0 0 # n (numero de la iteracion)
    
    # Valor actual del coseno (cos(x)) para n
    li t0 0
    fcvt.s.w fs0 t0

    # Sumando de la iteracion acutal n (new_iteration)
    li t0 1
    fcvt.s.w fs1 t0
    
    # Cargar 0.001 en un registro de coma flotante (fs2)
    li t0 0x3A83126F
    fmv.w.x fs2 t0
    
    # Guardar x en un registro s
    fmv.s fs3 fa0
    
    li s1 1 # Guardar el valor 1 para poder comparar luego con el resultado de flt.s
    
    sin_while:
    	fabs.s fs1 fs1 # Valor absoluto de new_interation
        
    	# Si new_iteration es menor que el error permitido (0.001), salta al final del bucle
    	flt.s t0 fs1 fs2 # Si fs1 < fs2 then t0 <- 0 else, t0 <- 1
        beq t0 s1 sin_fin
        
        # Realizar el factorial de 2n y lo deja en coma-flotante en ft0
        li t0 2
        mul a0 s0 t0
        addi a0 a0 1
        addi sp sp -4
		sw ra 0(sp)
        jal ra factorial
        lw ra 0(sp)
        addi sp sp 4
        
        fcvt.s.w fs4 a0
        
        
      # Calcular x^(2n)
        # pasar 2n al registro a0
        li t0 2
        mul a0 t0 s0 # tenemos 2n en a0
        addi a0 a0 1
        
        # pasar x al registro fa0
        fmv.s fa0 fs3
        
        addi sp sp -4
        sw ra 0(sp)
        jal ra potencia
        lw ra 0(sp)
        addi sp sp 4
        
        fmv.s fs5 fa0
        
        
        # Calcular (-1)^n utilizando un condicional
        li t0 2
        rem t1 s0 t0
        li t2 0
        
        beq t1 t2 sin_if # n % 2 == 0
            li t0 -1
            j sin_endif
        
        sin_if: 
            li t0 1
            
        sin_endif:
        
        # Pasar t0 a un registro de coma flotante
        fcvt.s.w ft0 t0     
        
        
        # Calcular cosx para esta iteración
        fmul.s fs5 ft0 fs5
        fdiv.s fs1 fs5 fs4
        
        fadd.s fs0 fs0 fs1
        
        addi s0 s0 1
                
        j sin_while
    
    sin_fin:
    fmv.s fa0 fs0
    

    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    flw fs0 12(sp)
    flw fs1 16(sp)
    flw fs2 20(sp)
    flw fs3 24(sp)
    flw fs4 28(sp)
    flw fs5 32(sp)
    addi sp sp 36
    
    jr ra
    
SinMatrix: # a0 = dirección A, a1 = dirección B, a2 = N (numero filas), a3 = M (numero columnas)

	addi sp sp -16
    sw s0 0(sp)
    sw s1 4(sp)
    sw s2 8(sp)
    sw s3 12(sp)
    
    # Guardo en registros s las direcciones de las matrices
    mv s0 a0
    mv s1 a1
    
    li s2 0 # s2 es el iterador
  	mul s3 a2 a3
    li t0 4 # s3 almacena el total de bytes en la matriz
    mul s3 s3 t0
    
    matrix_while: bge s2 s3 matrix_fin
    
    	add t0 s0 s2 # s4 almacena la dirección de memoria donde se encuentra el elemento actual
        flw fa0 0(t0) # Lee el valor del elemento de la matriz A

        # Calcula el valor del seno
        addi sp sp -4
        sw ra 0(sp)
        jal ra sin
        lw ra 0(sp)
        addi sp sp 4
		
        add t0 s1 s2
        fsw fa0 0(t0) # Guarda el valor del seno en la matriz B

        addi s2 s2 4
        j matrix_while
    
    matrix_fin:
    
    lw s0 0(sp)
    lw s1 4(sp)
    lw s2 8(sp)
    lw s3 12(sp)
    addi sp sp 16
	
	fin_programa:
	li t0 1