def factorial(n: int) -> int:
    factorial = 1
    i = 2

    while i <= n:
        factorial *= i
        i += 1
    
    return factorial


def potencia(n: float, exp: int) -> float:

    original = n
    iterar = 1

    if exp == 0:
        return iterar 

    while not iterar == exp:
        n *= original
        iterar += 1

    return n
def abs(n:float) -> float:
    if n < 0:
        return -n
    else:
        return n


def E() -> float:

    n = 0
    e = 0
    new_iteration = 1 # Se utiliza para la condición del bucle while

    # Repetir bucle hasta que se obtenga la precisión pedida.
    while abs(new_iteration) > 0.001:

        # Calcular el factorial de n
        n_fact = factorial(n)
        
        # Calcular e para el valor actual de n
        new_iteration = 1/n_fact
        e += new_iteration
        
        n += 1

    return e


def sin(x: float) -> float:

    n = 0
    sinx = 0
    new_iteration = 1 # Se utiliza para la condición del bucle while

    # Repetir bucle hasta que se obtenga la precisión pedida.
    while abs(new_iteration) > 0.001:

        # Calcular el factorial de 2n + 1
        sin_factorial = factorial(2*n + 1)
        
        # Calcular valor del seno para el valor actual de n
        new_iteration = ((-1)**n * x**(2*n + 1))/(sin_factorial)
        sinx += new_iteration
        
        n += 1

    return sinx

def cos(x: float) -> float:

    n = 0
    cosx = 0
    new_iteration = 1 # Se utiliza para la condición del bucle while

    # Repetir bucle hasta que se obtenga la precisión pedida.
    while abs(new_iteration) > 0.001:

        # Calcular el factorial de 2n 
        cos_factorial = factorial(2*n)
        
        # Calcular valor del seno para el valor actual de n
        new_iteration = ((-1)**n * x**(2*n))/(cos_factorial)
        cosx += new_iteration
        
        n += 1

    return cosx

def tg(x: float) -> float:
    return sin(x) / cos(x)

####
print(cos(2.0))
print(tg(0.5))