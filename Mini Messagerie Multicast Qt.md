## Mini Messagerie Multicast Qt

Un petit logiciel qui se base sur UDP et Qt pour permettre de s'envoyer des messages, des images ou des fichiers.

![image-20220423213237518](https://user-images.githubusercontent.com/82091053/164944294-42d2ecc8-0892-4b93-9956-7d7db65a8017.png)




### Aperçu de Qt

<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/0/0b/Qt_logo_2016.svg/langfr-220px-Qt_logo_2016.svg.png" alt="Description de l'image Qt logo 2016.svg." style="zoom:33%;" />

Qt est une API portable, orientée objet et développée en C++. Elle est constituée d'une multitude de composantes telles que la GUI,  les bases de données ou les réseaux. C'est donc exactement ce qu'il nous faut pour développer une messagerie !



### Multicast UDP

Le protocole *UDP* (User Datagram Protocol) permet aux applications d'échanger des datagrammes. Celui-ci fournit un service en mode non-connecté et sans reprise sur erreur.

Le multipoint (*Multicast*) est un mode diffusion permettant d'adresser des groupes de machines tout en étant plus sélectif qu'une diffusion (*Broadcast*). Pour cela on utilise des adresses IP de classe D.



