# Artificial Intelligence - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos da disciplina de **Inteligência Artificial (Artificial Intelligence - AI)** em inglês e português, com explicações e exemplos práticos.

---

## Conceitos Fundamentais

- **Artificial Intelligence (AI)** (Inteligência Artificial): Área da ciência da computação que busca criar sistemas capazes de realizar tarefas que normalmente exigiriam inteligência humana.

- **Machine Learning (ML)** (Aprendizado de Máquina): Subárea da IA que desenvolve algoritmos que aprendem a partir de dados.

- **Deep Learning (DL)** (Aprendizado Profundo): Subárea do aprendizado de máquina que utiliza redes neurais com múltiplas camadas.

- **Neural Network** (Rede Neural): Estrutura computacional inspirada no funcionamento do cérebro humano, composta por neurônios artificiais.

---

## Tipos de Aprendizado

- **Supervised Learning** (Aprendizado Supervisionado): O modelo aprende a partir de dados rotulados.

  Exemplo em Python com Scikit-learn:

  ```python
  from sklearn.linear_model import LogisticRegression
  from sklearn.datasets import load_iris

  X, y = load_iris(return_X_y=True)
  model = LogisticRegression(max_iter=200)
  model.fit(X, y)
  print(model.predict([[5.1, 3.5, 1.4, 0.2]]))
  ```

**Unsupervised Learning** (Aprendizado Não Supervisionado): O modelo identifica padrões em dados não rotulados.

  ```python
  from sklearn.cluster import KMeans
  from sklearn.datasets import load_iris

  X, _ = load_iris(return_X_y=True)
  kmeans = KMeans(n_clusters=3, random_state=42).fit(X)
  print(kmeans.labels_)
  ```

**Reinforcement Learning** (Aprendizado por Reforço): O agente aprende por tentativa e erro, recebendo recompensas ou punições.

---

## Conceitos Importantes

**Dataset** (Conjunto de Dados): Coleção de exemplos usada para treinar e avaliar modelos de IA.

**Training Set** (Conjunto de Treinamento): Subconjunto de dados usado para treinar o modelo.

**Test Set** (Conjunto de Teste): Subconjunto usado para avaliar a performance do modelo.

**Feature** (Atributo): Característica ou variável usada para representar os dados.

**Label** (Rótulo): Valor esperado associado a cada exemplo no aprendizado supervisionado.

**Overfitting** (Sobreajuste): Quando o modelo se ajusta demais aos dados de treinamento e perde capacidade de generalização.

**Underfitting** (Subajuste): Quando o modelo não consegue capturar padrões dos dados.

---

## Algoritmos Populares

**Decision Tree** (Árvore de Decisão): Estrutura em forma de árvore para tomada de decisões.

**Support Vector Machine (SVM)** (Máquina de Vetores de Suporte): Algoritmo para classificação e regressão baseado em margens.

**k-Nearest Neighbors (k-NN)** (k-Vizinhos Mais Próximos): Classificação baseada na proximidade de instâncias conhecidas.

**Linear Regression** (Regressão Linear): Modelo estatístico para prever valores numéricos.

  ```python
  from sklearn.linear_model import LinearRegression

  X = [[1], [2], [3], [4]]
  y = [2, 4, 6, 8]
  model = LinearRegression().fit(X, y)
  print(model.predict([[5]]))  # Saída: aproximadamente 10
  ```

---

## Deep Learning

**Perceptron** (Perceptron): Unidade básica de uma rede neural.

**Activation Function** (Função de Ativação): Função que define a saída de um neurônio (ex: ReLU, Sigmoid, Softmax).

**Backpropagation** (Retropropagação): Algoritmo de ajuste de pesos em redes neurais.

**Convolutional Neural Network (CNN)** (Rede Neural Convolucional): Usada principalmente em visão computacional.

**Recurrent Neural Network (RNN)** (Rede Neural Recorrente): Usada em processamento de linguagem natural e séries temporais.

---

## Avaliação de Modelos

**Accuracy** (Acurácia): Proporção de predições corretas.

**Precision** (Precisão): Proporção de predições positivas corretas.

**Recall** (Revocação): Proporção de positivos identificados corretamente.

**F1-Score** (F1-Score): Média harmônica entre precisão e recall.

  ```python
  from sklearn.metrics import classification_report

  y_true = [0, 1, 1, 0, 1]
  y_pred = [0, 1, 0, 0, 1]
  print(classification_report(y_true, y_pred))
  ```

---

## Aplicações de IA

**Natural Language Processing (NLP)** (Processamento de Linguagem Natural): Análise e geração de linguagem humana.

**Computer Vision** (Visão Computacional): Reconhecimento de imagens e vídeos.

**Speech Recognition** (Reconhecimento de Fala): Conversão de voz em texto.

**Recommendation Systems** (Sistemas de Recomendação): Sugerem produtos, músicas ou conteúdos personalizados.

---
