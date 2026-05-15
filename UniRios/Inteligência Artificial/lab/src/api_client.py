"""
Cliente da API USGS Earthquake.

Construído ao vivo em sala — começamos com `fetch_earthquakes` simples
e depois evoluímos para parsing limpo, tratamento de erros e cache.

Documentação oficial: https://earthquake.usgs.gov/fdsnws/event/1/
"""
from __future__ import annotations

from datetime import datetime, timedelta, timezone
from typing import Optional

import pandas as pd
import requests

USGS_URL = "https://earthquake.usgs.gov/fdsnws/event/1/query"

# Boa prática profissional: APIs públicas pedem identificação na requisição.
# Sem User-Agent, alguns provedores retornam 403.
HEADERS = {
    "User-Agent": "AulaAI/1.0 (contato: contato@aulai.com)",
    "Accept": "application/json",
}


def fetch_earthquakes(
    days_back: int = 7,
    min_magnitude: float = 2.5,
    limit: int = 1000,
) -> pd.DataFrame:
    """
    Busca terremotos do USGS e retorna um DataFrame pronto para análise.

    Args:
        days_back: Quantos dias para trás buscar (padrão: última semana).
        min_magnitude: Magnitude mínima (filtra ruído sísmico).
        limit: Máximo de eventos retornados.

    Returns:
        DataFrame com colunas: time, place, magnitude, depth_km, lat, lon, url, tsunami, type.
    """
    end_time = datetime.now(timezone.utc)
    start_time = end_time - timedelta(days=days_back)

    params = {
        "format": "geojson",
        "starttime": start_time.strftime("%Y-%m-%d"),
        "endtime": end_time.strftime("%Y-%m-%d"),
        "minmagnitude": min_magnitude,
        "limit": limit,
        "orderby": "time",  # mais recentes primeiro
    }

    response = requests.get(USGS_URL, params=params, headers=HEADERS, timeout=30)
    response.raise_for_status()

    data = response.json()
    return _parse_geojson(data)


def _parse_geojson(geojson: dict) -> pd.DataFrame:
    """Transforma o GeoJSON da USGS em um DataFrame tabular limpo."""
    registros = []
    for feature in geojson.get("features", []):
        props = feature["properties"]
        coords = feature["geometry"]["coordinates"]  # [lon, lat, depth]

        registros.append({
            "time": pd.to_datetime(props["time"], unit="ms"),
            "place": props.get("place") or "Localização desconhecida",
            "magnitude": props.get("mag"),
            "depth_km": coords[2],
            "lat": coords[1],
            "lon": coords[0],
            "url": props.get("url"),
            "tsunami": bool(props.get("tsunami", 0)),
            "type": props.get("type", "earthquake"),
        })

    df = pd.DataFrame(registros)

    if not df.empty:
        # Limpeza básica que sempre fazemos
        df = df.dropna(subset=["magnitude", "lat", "lon"])
        df = df.sort_values("time", ascending=False).reset_index(drop=True)

        # Feature engineering simples — útil no dashboard
        df["region"] = df["place"].str.split(",").str[-1].str.strip()
        df["hour"] = df["time"].dt.hour
        df["date"] = df["time"].dt.date

    return df


def get_summary_stats(df: pd.DataFrame) -> dict:
    """KPIs prontos para o dashboard."""
    if df.empty:
        return {"total": 0, "max_mag": 0, "avg_mag": 0, "tsunamis": 0, "latest": None}

    return {
        "total": len(df),
        "max_mag": float(df["magnitude"].max()),
        "avg_mag": float(df["magnitude"].mean()),
        "tsunamis": int(df["tsunami"].sum()),
        "latest": df.iloc[0]["time"],
    }


if __name__ == "__main__":
    # Smoke test — rode `python src/api_client.py` para validar que tudo funciona
    print("Buscando terremotos dos últimos 7 dias...")
    df = fetch_earthquakes(days_back=7, min_magnitude=4.0)
    print(f"{len(df)} terremotos encontrados\n")
    print(df[["time", "place", "magnitude"]].head(10).to_string())
    print(f"\nStats: {get_summary_stats(df)}")
