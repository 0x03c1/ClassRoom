# Pipeline Completo com API ao Vivo

> **Objetivo da aula**: construir, do zero ao deploy, um pipeline de dados

## Fonte de dados: USGS Earthquake API

A **United States Geological Survey** publica gratuitamente dados de **todos os terremotos
detectados no planeta**, atualizados em tempo quase real. Não exige cadastro, chave ou
autenticação — a melhor API pública para ensinar.

- **Endpoint**: `https://earthquake.usgs.gov/fdsnws/event/1/query`
- **Formato**: GeoJSON
- **Cobertura**: global, milhares de eventos por dia
- **Latência**: ~1 minuto entre o evento físico e a publicação

## O que vamos construir

Um app **Streamlit** com 3 abas:

1. **Visão Geral** — KPIs (total, magnitude máxima, mais recente) e tabela
2. **Mapa Mundial** — terremotos plotados, tamanho proporcional à magnitude
3. **Análises** — distribuições, top regiões, padrões temporais

## Como rodar

```bash
pip install -r requirements.txt
streamlit run app.py
```

A primeira aba carrega em ~3 segundos. Sem credenciais, sem variáveis de ambiente.

## Estrutura

```
Api/
├── app.py                      # Dashboard Streamlit (rode com: streamlit run app.py)
├── src/
│   └── api_client.py           # Cliente da API USGS (reusável)
├── requirements.txt
└── README.md
```

## Por que isso vale dinheiro no mercado

Esse exato padrão — **API pública - análise - dashboard** — é o que empresas pagam para
freelancers e analistas júnior fazerem. Exemplos reais:

- Monitorar **APIs governamentais** (Banco Central, IBGE, INPE) para clientes B2B
- **Web scraping de e-commerce** + dashboard de preços para varejistas
- **APIs de redes sociais** + análise de sentimento para agências de marketing

Se você sabe fazer esse pipeline, já tem o suficiente para um **portfólio cobrável** de
R$ 500 a R$ 3.000 por projeto pequeno no Workana / Upwork :)
