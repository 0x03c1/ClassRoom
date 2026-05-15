import streamlit as st
import pandas as pd
import plotly.express as px

from src.api_client import fetch_earthquakes, get_summary_stats

# --- Configuração da página ---
st.set_page_config(
    page_title="Terremotos em Tempo Real",
    layout="wide",
)


# --- Cache da API (evita bater a cada interação) ---
@st.cache_data(ttl=300)  # 5 minutos
def carregar_dados(dias: int, mag_min: float) -> pd.DataFrame:
    """Wrapper com cache para a API. ttl=300 = recarrega a cada 5min."""
    return fetch_earthquakes(days_back=dias, min_magnitude=mag_min)


# --- Sidebar com filtros ---
st.sidebar.title("Filtros")

dias = st.sidebar.slider(
    "Últimos N dias",
    min_value=1, max_value=30, value=7,
    help="USGS oferece histórico até décadas, mas seguramos em 30 dias por performance.",
)

mag_min = st.sidebar.slider(
    "Magnitude mínima",
    min_value=0.0, max_value=8.0, value=4.0, step=0.5,
    help="Magnitude < 2.5 é praticamente imperceptível. > 6.0 é considerado forte.",
)

if st.sidebar.button("Atualizar agora"):
    st.cache_data.clear()
    st.rerun()

st.sidebar.markdown("---")
st.sidebar.markdown(
    "**Fonte**: [USGS](https://earthquake.usgs.gov/fdsnws/event/1/) "
    "(United States Geological Survey)"
)


# --- Título principal ---
st.title("Terremotos em Tempo Real")
st.caption(
    "Dados atualizados a cada minuto pela USGS. "
    "Construído ao vivo na aula de IA — Prof. Roni."
)

# --- Carregar dados ---
with st.spinner(f"Buscando últimos {dias} dias na API USGS..."):
    df = carregar_dados(dias, mag_min)

if df.empty:
    st.warning("Nenhum terremoto encontrado com esses filtros. Reduza a magnitude mínima.")
    st.stop()

# --- KPIs no topo ---
stats = get_summary_stats(df)
col1, col2, col3, col4 = st.columns(4)
col1.metric("Total de eventos", f"{stats['total']:,}".replace(",", "."))
col2.metric("Magnitude máxima", f"{stats['max_mag']:.1f}")
col3.metric("Magnitude média", f"{stats['avg_mag']:.2f}")
col4.metric("Alertas de tsunami", stats['tsunamis'])

st.caption(
    f"Mais recente: **{stats['latest']:%d/%m/%Y %H:%M UTC}** "
    f"em *{df.iloc[0]['place']}*"
)

st.markdown("---")

# --- 3 abas ---
aba_geral, aba_mapa, aba_analises = st.tabs([
    "Visão Geral", "Mapa Mundial", "Análises"
])

# === Aba 1: Visão Geral ===
with aba_geral:
    st.subheader("Eventos mais recentes")
    st.dataframe(
        df[["time", "place", "magnitude", "depth_km", "tsunami", "url"]].head(50),
        column_config={
            "time": st.column_config.DatetimeColumn("Quando", format="DD/MM/YYYY HH:mm"),
            "place": "Local",
            "magnitude": st.column_config.NumberColumn("Magnitude", format="%.1f"),
            "depth_km": st.column_config.NumberColumn("Profundidade (km)", format="%.1f"),
            "tsunami": st.column_config.CheckboxColumn("Tsunami?"),
            "url": st.column_config.LinkColumn("Detalhes USGS"),
        },
        hide_index=True,
        use_container_width=True,
    )

# === Aba 2: Mapa ===
with aba_mapa:
    st.subheader("Distribuição geográfica")

    # Magnitude exponencial → tamanho do ponto. Sismos seguem escala logarítmica
    # (Richter), então elevamos ao quadrado para diferença visível no mapa.
    df_mapa = df.copy()
    df_mapa["size"] = df_mapa["magnitude"] ** 2

    fig_mapa = px.scatter_geo(
        df_mapa,
        lat="lat", lon="lon",
        color="magnitude",
        size="size",
        hover_name="place",
        hover_data={"magnitude": ":.1f", "depth_km": ":.1f", "size": False, "lat": False, "lon": False},
        color_continuous_scale="Reds",
        projection="natural earth",
        title=f"{len(df)} terremotos nos últimos {dias} dias (magnitude ≥ {mag_min})",
    )
    fig_mapa.update_layout(height=600, margin=dict(l=0, r=0, t=40, b=0))
    st.plotly_chart(fig_mapa, use_container_width=True)

    st.info(
        "**Observação para a turma**: notem como os pontos se concentram nas "
        "bordas das placas tectônicas — Cinturão de Fogo do Pacífico, Mediterrâneo, "
        "Andes. Os dados *contam a tectônica de placas* sozinhos."
    )

# === Aba 3: Análises ===
with aba_analises:
    col_a, col_b = st.columns(2)

    with col_a:
        st.subheader("Distribuição de magnitudes")
        fig_hist = px.histogram(
            df, x="magnitude", nbins=30,
            color_discrete_sequence=["steelblue"],
        )
        fig_hist.update_layout(yaxis_title="Quantidade", xaxis_title="Magnitude")
        st.plotly_chart(fig_hist, use_container_width=True)

    with col_b:
        st.subheader("Magnitude Profundidade")
        fig_scatter = px.scatter(
            df, x="depth_km", y="magnitude",
            color="magnitude", color_continuous_scale="Reds",
            opacity=0.6,
        )
        fig_scatter.update_layout(xaxis_title="Profundidade (km)")
        st.plotly_chart(fig_scatter, use_container_width=True)

    st.subheader("Top 10 regiões com mais terremotos")
    top_regioes = (
        df["region"].value_counts().head(10).reset_index()
        .rename(columns={"region": "Região", "count": "Eventos"})
    )
    fig_regioes = px.bar(
        top_regioes, x="Eventos", y="Região",
        orientation="h", color="Eventos", color_continuous_scale="Blues",
    )
    fig_regioes.update_layout(yaxis={"categoryorder": "total ascending"})
    st.plotly_chart(fig_regioes, use_container_width=True)

    st.subheader("Eventos por dia")
    por_dia = df.groupby("date").size().reset_index(name="quantidade")
    fig_tempo = px.line(
        por_dia, x="date", y="quantidade",
        markers=True, color_discrete_sequence=["#2E86AB"],
    )
    fig_tempo.update_layout(xaxis_title="Data", yaxis_title="Eventos detectados")
    st.plotly_chart(fig_tempo, use_container_width=True)


# --- Rodapé ---
st.markdown("-")
st.caption(
    "Curso de IA — Construído com Streamlit + USGS API"
)
