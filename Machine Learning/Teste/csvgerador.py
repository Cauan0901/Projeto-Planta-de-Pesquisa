import pandas as pd
import numpy as np

# Configurações
tempo_total = 3000
intervalo_amostragem = 20
tempos = np.arange(0, tempo_total + intervalo_amostragem, intervalo_amostragem)

# Sinal (mA): Degraus em t=1s e t=2900s
sinal = np.piecewise(tempos, 
    [tempos < 1, (tempos >= 1) & (tempos < 2900), tempos >= 2900], 
    [0.0, 20.0, 0.0]
)

# Vazão (L/h): Degrau em t=2000s
vazao = np.piecewise(tempos, 
    [tempos < 2000, tempos >= 2000], 
    [50.0, 200.0]
)

# Temperatura (°C): Progressão linear
temperatura = np.linspace(20, 60, len(tempos))

# Criar DataFrame
df = pd.DataFrame({
    'Tempo (s)': tempos,
    'Sinal (mA)': sinal,
    'Vazão (L/h)': vazao,
    'Temperatura (°C)': temperatura
})

# Salvar como CSV
df.to_csv('dados_graficos_final.csv', index=False, decimal='.', float_format='%.1f')
print("Arquivo 'dados_graficos_final.csv' gerado com sucesso!")