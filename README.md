# Serviço de reservas de Hotel

## 🧠 Descrição
A aplicação "Hotel Reservation" simula um sistema de gerenciamento de reservas de quarto em um hotel. Foram utilizadas como fundamento estruturas de dados vista em sala de aula, na disciplina de Algoritmos e Estruturas de Dados II.

## 📌 Objetivo
O objetivo do projeto é garantir o gerenciamento otimizado e eficiente dos dados em disco.
- Busca por uma chave fornecida pelo cliente;
- Busca por um colaborador por meio de um ID único;
- Pesquisa por uma determinada reserva.

## 🧩 Entidades

1. **Colaborador**
   - `id`: Código identificador único.
   - `name`: Nome do colaborador.
   - `position`: Cargo em que o colaborador trabalha.

2. **Cliente**
   - `name`: Nome completo do cliente.
   - `cpf`: Documento de identificação único (chave de pesquisa).
   - `phone`: Número para contato.
   - `numReservation`: Número da reserva.

3. **Pedido**
   - `numRoom`: Número do quarto reservado.
   - `qntOfGuests`: Quantidade de hóspedes.
   - `periodOfStay`: Período da reserva.
   - `price`: Valor da reserva.
     
## 💻 Tecnologia Utilizadas
- C

## 📥 Repositório Clonado

```bash
https://github.com/Vitwra/Hotel_Reservation.git
```

## ✅ Como Executar

```bash
gcc main.c cliente.c reserva.c -o sistema
./sistema
