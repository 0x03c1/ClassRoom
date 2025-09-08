# Software Quality - Termos em Inglês e Português com Exemplos

Este guia apresenta os principais conceitos da área de **Qualidade de Software (Software Quality)** em inglês e português, com explicações e exemplos práticos.

---

## Conceitos Fundamentais

- **Software Quality** (Qualidade de Software): Grau em que um sistema, componente ou processo atende aos requisitos especificados e às expectativas do usuário.

- **Verification** (Verificação): Processo de garantir que o software foi construído corretamente, conforme especificações.

- **Validation** (Validação): Processo de garantir que o software atende às necessidades do usuário.

- **Quality Assurance (QA)** (Garantia da Qualidade): Conjunto de atividades que asseguram que os processos de desenvolvimento sigam padrões de qualidade.

- **Quality Control (QC)** (Controle de Qualidade): Conjunto de atividades focadas em identificar defeitos no produto final.

---

## Testes de Software

- **Unit Test** (Teste Unitário): Testa partes isoladas do código (funções, métodos ou classes).

  ```python
  import unittest

  def soma(a, b):
      return a + b

  class TesteSoma(unittest.TestCase):
      def test_soma(self):
          self.assertEqual(soma(2, 3), 5)

  if __name__ == "__main__":
      unittest.main()
      ```

**Integration Test** (Teste de Integração): Testa a interação entre diferentes módulos do sistema.

**System Test** (Teste de Sistema): Testa o sistema completo como um todo.

**Acceptance Test** (Teste de Aceitação): Testa se o sistema atende aos requisitos do cliente.

**Regression Test** (Teste de Regressão): Garante que novas alterações não quebraram funcionalidades já existentes.

---

## Tipos de Teste

**Functional Test** (Teste Funcional): Valida o que o sistema faz de acordo com os requisitos.

**Non-Functional Test** (Teste Não Funcional): Valida atributos como desempenho, usabilidade e segurança.

**Performance Test** (Teste de Desempenho): Mede a velocidade, escalabilidade e estabilidade do sistema.

  ```bash
  ab -n 1000 -c 100 http://localhost:8000/
  ```

**Load Test** (Teste de Carga): Verifica o comportamento do sistema sob alta demanda.

**Stress Test** (Teste de Estresse): Testa os limites do sistema em condições extremas.

**Usability Test** (Teste de Usabilidade): Avalia a experiência do usuário ao interagir com o software.

**Security Test** (Teste de Segurança): Avalia se o sistema está protegido contra ataques e acessos não autorizados.

---

## Automação de Testes

**Test Automation** (Automação de Testes): Uso de ferramentas para executar testes automaticamente.

  Exemplo em Python com Selenium:

  ```python
  from selenium import webdriver

  driver = webdriver.Chrome()
  driver.get("http://example.com")
  assert "Example" in driver.title
  driver.quit()
  ```

**Continuous Integration (CI)** (Integração Contínua): Processo de integração frequente de código em um repositório, com testes automatizados.

**Continuous Delivery (CD)** (Entrega Contínua): Processo de disponibilizar o software de forma automatizada e confiável em produção.

---

## Métricas de Qualidade

**Code Coverage** (Cobertura de Código): Percentual do código que foi executado durante os testes.

  ```bash
  pytest --cov=meu_modulo
  ```

**Defect Density** (Densidade de Defeitos): Número de defeitos por tamanho do software (ex: defeitos por 1000 linhas de código).

**MTBF - Mean Time Between Failures** (Tempo Médio Entre Falhas): Mede a confiabilidade do sistema.

**MTTR - Mean Time To Repair** (Tempo Médio para Reparo): Tempo médio necessário para corrigir falhas.

---

## Normas e Modelos de Qualidade

**ISO/IEC 25010** (ISO/IEC 25010): Modelo de qualidade de software que define características como funcionalidade, confiabilidade, usabilidade, eficiência, manutenibilidade e portabilidade.

**CMMI - Capability Maturity Model Integration** (CMMI - Modelo Integrado de Maturidade de Capacidades): Modelo para avaliação da maturidade dos processos de software.

**MPS.BR** (Melhoria de Processo do Software Brasileiro): Modelo de qualidade adaptado à realidade brasileira.

---

## Ferramentas de Qualidade

**SonarQube**: Análise estática de código para identificar problemas de qualidade e segurança.

**Jenkins**: Ferramenta de automação para CI/CD.

**JUnit / PyTest / NUnit**: Frameworks de testes unitários.

**Postman**: Testes de API.

**JMeter**: Testes de carga e desempenho.

---
