# Estudo de Sistemas Embarcados

- **Arquitetura do ESP32 e Ambiente de Desenvolvimento** ✅
- **Programação de Tarefas e Conceitos Básicos do FreeRTOS** ✅
- **Protocolos de Comunicação: UART, I²C e SPI** 🔍
- **Conexão Wi-Fi e Redes (HTTP, MQTT Básico)** 🔜
- **Integração com AWS IoT Core** 🔜
- **Comparativo entre Ambientes de Desenvolvimento: ESP-IDF vs PlatformIO** 🔜
- **Tasmota: Uso Básico e Integração MQTT** 🔜
- **Depuração Básica e Boas Práticas Iniciais** 🔜
- **FreeRTOS Avançado e Gerenciamento de Multitarefas** 🔜
- **Tasmota Avançado: Personalização e Compilação** 🔜
- **AWS IoT: Conceitos Avançados e Segurança** 🔜
- **Visualização de Dados com Grafana** 🔜
- **Depuração Avançada e Técnicas de Otimização** 🔜
- **Boas Práticas Avançadas de Desenvolvimento de Firmware** 🔜
- **Projeto Final Integrado** 🔜

--- 

# Exercícios Práticos

- **`blink`**: Exercício básico para acionar um LED de forma intermitente.
  
- **`basicTasks`**: Exercício de introdução ao uso de tarefas (tasks).
    - Consiste na criação de duas tarefas: uma que acende um LED a cada 1 segundo e outra que imprime "Hello World!" no terminal.

- **`priorityTasks`**: Exercício de tarefas com diferentes prioridades.
    - Este exercício utiliza três tarefas que emitem sinais para LEDs com intervalos de 250ms, 500ms e 1000ms. A prioridade das tarefas garante que a tarefa de maior prioridade emita o sinal primeiro, mesmo que elas coincidam no tempo.

- **`tasksWithQueues`**: Exercício de comunicação entre tarefas utilizando filas.
    - Criação de uma fila onde uma tarefa envia valores para a fila e outra tarefa retira e exibe esses valores no terminal.

- **`tasksWithSemaphores`**: Em progresso...

---
