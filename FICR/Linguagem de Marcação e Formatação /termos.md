# Markup and Styling Languages (HTML and CSS) - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos da disciplina de **Linguagem de Marcação e Formatação (Markup and Styling Languages)** em inglês e português, com explicações e exemplos práticos em **HTML e CSS**.

---

## Conceitos Fundamentais

- **HTML - HyperText Markup Language** (Linguagem de Marcação de Hipertexto): Linguagem usada para estruturar o conteúdo de páginas web.

- **CSS - Cascading Style Sheets** (Folhas de Estilo em Cascata): Linguagem usada para definir a apresentação visual (cores, fontes, layout) do conteúdo em HTML.

- **Tag** (Tag): Elemento básico do HTML que define a estrutura do documento.

  ```html
  <p>Este é um parágrafo.</p>
  ```

**Attribute** (Atributo): Informação adicional dentro de uma tag HTML.

  ```html
  <img src="imagem.jpg" alt="Descrição da imagem">
  ```

**Selector** (Seletor): Elemento em CSS usado para aplicar estilos a partes específicas do HTML.

  ```css
  p {
    color: blue;
  }
  ```

---

## Estrutura Básica de um Documento HTML

**DOCTYPE** (Declaração do Tipo de Documento): Define a versão do HTML.

**Head** (Cabeçalho): Contém metadados, título e links para recursos.

**Body** (Corpo): Contém o conteúdo visível da página.

  ```html
  <!DOCTYPE html>
  <html>
  <head>
      <title>Minha Página</title>
  </head>
  <body>
      <h1>Bem-vindo ao HTML</h1>
      <p>Este é um exemplo simples.</p>
  </body>
  </html>
  ```

---

## Estruturas Comuns em HTML

**Headings** (Títulos): Estruturas de título de diferentes níveis.

  ```html
  <h1>Título Principal</h1>
  <h2>Subtítulo</h2>
  ```

**Paragraph** (Parágrafo):

  ```html
  <p>Este é um parágrafo de texto.</p>
  ```

**Lists** (Listas):

  ```html
  <ul>
      <li>Item 1</li>
      <li>Item 2</li>
  </ul>
  ```

**Links** (Hiperlinks):

  ```html
  <a href="https://www.exemplo.com">Visite o site</a>
  ```

**Images** (Imagens):

  ```html
  <img src="foto.jpg" alt="Foto de exemplo">
  ```

**Tables** (Tabelas):

  ```html
  <table border="1">
      <tr>
          <th>Nome</th>
          <th>Idade</th>
      </tr>
      <tr>
          <td>Ana</td>
          <td>20</td>
      </tr>
  </table>
  ```

---

## CSS: Seletores e Propriedades

**Inline CSS** (CSS em linha):

  ```html
  <p style="color: red;">Texto em vermelho</p>
  ```

**Internal CSS** (CSS interno):

  ```html
  <style>
      body {
          background-color: lightgray;
      }
  </style>
  ```

**External CSS** (CSS externo):

  ```html
  <link rel="stylesheet" href="estilos.css">
  ```

---

## Estilização com CSS

**Colors** (Cores):

  ```css
  h1 {
      color: darkgreen;
  }
  ```

**Fonts** (Fontes):

  ```css
  p {
      font-family: Arial, sans-serif;
      font-size: 16px;
  }
  ```

**Box Model** (Modelo de Caixa):

  ```css
  div {
      margin: 10px;
      padding: 20px;
      border: 1px solid black;
  }
  ```

**Layout com Flexbox**:

  ```css
  .container {
      display: flex;
      justify-content: center;
      align-items: center;
  }
  ```

**Layout com Grid**:

  ```css
  .grid {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      gap: 10px;
  }
  ```

---
