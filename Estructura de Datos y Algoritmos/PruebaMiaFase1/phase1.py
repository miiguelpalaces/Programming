from slistH import SList
from slistH import SNode

import sys

class SList2(SList):

    
    def delLargestSeq(self): 
        #implement here your solution

        if not self.isEmpty():
            # si la lista no está vacía
            if self._size == 1:
                # si solo hay un elemento
                self.removeFirst()
            else:
                aux = self._head

                # variables universales
                comienzo_max = aux
                cant_max = 1

                # variables locales
                comienzo_aux = aux
                cant_aux = 1

                fin = False

                while not fin:
                    aux = aux.next

                    if aux == None:
                        # hemos llegado al final
                        if cant_aux >= cant_max:
                            # actualizamos
                            comienzo_max = comienzo_aux
                            cant_max = cant_aux
                        fin = True
                    else:
                        if aux.elem == comienzo_aux.elem:
                            # 2 elementos contiguos son iguales
                            cant_aux += 1
                        else:
                            if cant_aux >= cant_max:
                                # actualizamos
                                comienzo_max = comienzo_aux
                                cant_max = cant_aux
                            
                            
                            comienzo_aux = comienzo_aux.next
                            cant_aux = 1
                
                # itero hasta el nodo previo al comienzo de la sección que voy a eliminar
                prev = self._head
                while prev.next != comienzo_max:
                    prev = prev.next
        
                hilo1 = prev
                
                # itero hasta el siguiente nodo del último de la sección que quiero eliminar
                for i in range(cant_max):
                    prev = prev.next
                
                # eliminamos la secuencia
                hilo1.next = prev.next
                self._size -= cant_max


    def fix_loop(self):
        #implement here your solution

        if self.isEmpty():
            # si la lista está vacía
            return False
        else:
            # creamos una lista auxiliar
            lista_aux = SList2()
            # guardamos el nodo de la lista original
            lista_aux.addFirst(self._head)

            aux = self._head

            find= False

            while aux != None:  
                # revisa toda la lista
                prev = aux
                aux = aux.next

                if lista_aux.check_in_list(aux):
                    # hemos encontrado el bucle
                    prev.next = None
                    return True
                else:
                    # añadimos el bucle a la lista auxiliar
                    lista_aux.addLast(aux)
            
            return False
    
    def check_in_list(self, nodo: SNode):
        """ Revisa si el nodo ya se encuentra en la lista """

        aux = self._head

        while aux != None:
            # iteramos la lista hasta el final
            if aux.elem == nodo:
                # el nodo se encuentra en la lista
                return True
            
            aux = aux.next

        return False


    def create_loop(self, position):
        # this method is used to force a loop in a singly linked list
        if position < 0 or position > len(self) - 1:
            raise ValueError(f"Position out of range [{0} - {len(self) - 1}]")

        current = self._head
        i = 0

        # We reach position to save the reference
        while current and i < position:
            current = current.next
            i += 1

        # We reach to tail node and set the loop
        start_node = current
        print(f"Creating a loop starting from {start_node.elem}")
        while current.next:
            current = current.next        
        current.next = start_node
		
		
	
    def leftrightShift(self,left,n):
        #implement here your solution

        if not self.isEmpty():
            # si la lista no está vacía
            if self._size > 1:
                # si hay más de un elemento
                if left: # left == True
                    if n == 1:
                        # si quiere mover el primer elemento
                        first = self.removeFirst()
                        self.addLast(first)
                    else:
                        if n < self._size:
                            # hay más elementos, además de la sección a mover
                            aux = self._head
                            start_section = self._head  # comienzo de la sección que tenemos que mover
                            for i in range(n-1):
                                aux = aux.next
                            last_section = aux     # final de la sección que tenemos que mover
                            aux = aux.next
                            last_section.next = None    # el nuevo último apunta a None
                            self._head = aux
                            while aux.next != None:
                                aux = aux.next
                            aux.next = start_section   # el último de la lista original apunta al primero de la sección
                
                else: # left == False
                    if n == 1:
                        # si quiere mover el último elemento
                        last = self.removeLast()
                        self.addFirst(last)
                    else:
                        if n <  self._size:
                            # hay más elementos, además de la sección a mover
                            aux = self._head
                            for i in range(self._size - n -1):
                                aux = aux.next
                            new_last = aux  # nuevo último
                            aux = aux.next
                            new_last.next = None
                            start_section = aux   # comienzo de la sección a mover
                            while aux.next != None:
                                aux = aux.next
                            aux.next = self._head
                            self._head = start_section  # el primero de la sección, se convierte en el head


if __name__=='__main__':
    
    l=SList2()
    print("list:",str(l))
    print("len:",len(l))

    for i in range(7):
        l.addLast(i+1)

    print(l)
    print()

    l=SList2()
    print("list:",str(l))
    print("len:",len(l))

    for i in range(7):
        l.addLast(i+1)

    print(l)
    print()

    # No loop yet, no changes applied
    l.fix_loop()
    print("No loop yet, no changes applied")
    print(l)
    print()

    # We force a loop
    l.create_loop(position=6)
    l.fix_loop()
    print("Loop fixed, changes applied")
    print(l)
    print()
    print()

    l = SList2()
    for i in [1,2,3,4,5,]:        
        l.addLast(i)
    print(l.delLargestSeq())


    l=SList2()
    for i in range(7):
         l.addLast(i+1)

    print(l)
    l.leftrightShift(False, 2)
    print(l)
    
    
    