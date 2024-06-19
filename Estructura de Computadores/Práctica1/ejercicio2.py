from ejercicio1 import sin

def SinMatrix(A:list[list[float]], B:list[list], N:int, M:int) -> None:

    i = 0
    j = 0

    while i < N: # Iterar sobre las filas
        while j < M: # Iterar sobre cada elemento de la fila
            B[i][j] = sin(A[i][j])
            j += 1
        i += 1
    
    return 






