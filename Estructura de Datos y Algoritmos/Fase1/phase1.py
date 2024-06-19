from slistH import SList
from slistH import SNode

import sys

class Sequence:
    def __init__(self, prev=None, end=None, size=0):
        self.prev = prev
        self.end = end
        self.size = size

class SList2(SList):

    def delLargestSeq(self): 

        if not self.isEmpty(): # Control si lista está vacía

            previous_node, current_node = None, self._head # Crear pointers para iterar sobre la lista enlazada

            while current_node != None:
                
                if current_node == self._head: # Controlar si no hay lista creada (al ser la primera iteración)
                    current_seq = Sequence(self._head, self._head, 1)
                    largest_seq = current_seq

                else:

                    if current_node.elem == previous_node.elem:
                        # Si es el mismo elemento, aumentar la secuencia actual
                        current_seq.end = current_node
                        current_seq.size += 1

                    else:
                        # Si no es el mismo elemento, crear una secuencia nueva
                        current_seq = Sequence(previous_node, current_node, 1)

                # Comprobar si la lista actual supera a la más larga
                if current_seq.size >= largest_seq.size:
                    largest_seq = current_seq

                # Iterar sobre la lista enlazada
                previous_node = current_node
                current_node = current_node.next

            # Actualizar la lista, elminando la secuencia más larga
            if largest_seq.prev == self._head:
                # Si la secuencia más larga comienza en el primer elemento de la lista, actualizar head
                self._head = largest_seq.end.next

            else:
                largest_seq.prev.next = largest_seq.end.next

            # Actualizar el tamaño de la lista
            self._size -= largest_seq.size
                       


    def fix_loop(self):

        if not self.isEmpty(): # Controla en el caso que no haya ningún elemento en la lista

            nodes_visited = SList2() # Para guardar los nodos ya visitados
            current = self._head
            while current != None:

                if nodes_visited.in_slist(current.next):
                    # Apuntar a None el puntero que apunta hacia otro nodo de la lista
                    current.next = None
                    return True

                # Añadir a los nodos ya visitados el nodo actual
                nodes_visited.addFirst(current)

                current = current.next
                
            return False

    def in_slist(self, node: SNode):
        """Devuelve True si en la lista invocante se encuentra nodo (almacenado como elemento), en otro caso False."""

        current = self._head
        while current != None:

            if current.elem == node:
                return True

            current = current.next

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

        if n < self._size and n != 0:

            # Igualar n al n elemento de la lista contando por delante
            if not left:
                n = self._size - n
            
            # Iteradores sobre la SList2
            tail, i = self._head, 1

            while tail.next != None:
                
                # Si ya hemos llegado al último/primer elemento que debemos desplazar, guardar el puntero que se dirige a él
                if i == n:
                    # Guardar el nodo previo al nodo "n", para así poder luego acceder a ambos y reordenar la lista
                    important = tail

                #Iterar sobre la SList2
                tail = tail.next
                i+= 1

            # Una vez llegado al final de la lista, recolocar los n elementos
            tail.next = self._head
            if left:
                # Recolocar los n primeros SNodes al final
                self._head = important.next
                important.next = None

            else:
                # Recoloar los n últimos SNodes al principio
                self._head = important.next
                important.next = None  



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
    for i in [1,2,3,4,5]:        
        l.addLast(i)
    print(l.delLargestSeq())


    l=SList2()
    for i in range(7):
         l.addLast(i+1)

    print(l)
    l.leftrightShift(False, 2)
    print(l)