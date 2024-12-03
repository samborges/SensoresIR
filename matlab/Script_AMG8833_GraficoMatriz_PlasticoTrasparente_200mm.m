clc
clear all
close all

% Nome do arquivo CSV
filename = 'arrays\temperatura_AMG_8833\Array_DadosPlasticoLiquidoTransparente_200mm_AMG8833.txt';

numeroColuna = 36;
% Ler o arquivo CSV
data = readmatrix(filename, 'Delimiter', ';');
temperaturaColunaBruto = data(:,numeroColuna);

% Aplicar o filtro de média móvel com 5 posições
% para cada ponto na série de dados, o script calcula a média dos 10 valores anteriores a esse ponto,
% incluindo o ponto atual. Isso resulta em um novo vetor filtroMediaMovel onde cada valor é a média
% dos 10 valores anteriores a ele na série original.
filtroMediaMovel = movmean(data,[5 0]);

% Extrair a coluna mais relevante da coleta
colunaRelevante = filtroMediaMovel(:, numeroColuna);
temperaturaMediaMovel = colunaRelevante(end,:);
disp('Dados media móvel Ponto mais relevante');
disp(temperaturaMediaMovel);


% Calcular a média dos dados da quarta coluna
media_coluna = filtroMediaMovel(end,:);
% disp('Dados media móvel matrix 8X8:');
% disp(media_coluna);

% Verificar se temos pelo menos 64 dados para preencher a matriz 8x8
if numel(media_coluna) < 64
    error('Não há dados suficientes para preencher a matriz 8x8.');
end

% Selecionar os primeiros 64 dados da média móvel
dados_para_heatmap = media_coluna(1:64);

% Reformatar os dados em uma matriz 8x8
matriz_8x8 = reshape(dados_para_heatmap, [8, 8]);
matriz_8x8_Rotate = rot90(matriz_8x8, 1);

% Criar o mapa de calor
f1 = figure;
f1.Position(3:4) = [600 420];
heatmap(matriz_8x8_Rotate);
title('Sensor AMG 8833 (Plástico Transparente 200mm)- Mapa de Calor (°C)');
colormap('cool')
xlabel('Base');
ylabel('Laterais');

% Criar o gráfico de linha comparando os dados originais e filtrados
f2 = figure;
hold on;
f2.Position(3:4) = [800 420];
plot(temperaturaColunaBruto, 'b-.', 'Color', [0.0, 0.0, 1], 'LineWidth', 0.5,'DisplayName', 'Dados Originais');
plot(colunaRelevante, '-', 'Color', [0.00,1.00,1.00], 'LineWidth', 1, 'DisplayName', 'Média Móvel');
yline(temperaturaMediaMovel, '-', 'Color', [0.0, 0.0, 0.0], 'LineWidth', 2, 'DisplayName', 'Valor Médio', 'Alpha',0.5);

temperaturaCamera = 48.5;
temperaturaTermometro = 49;

% Adicionar o valor médio no gráfico
x_position = length(colunaRelevante)/2; % Posição X para o texto
y_position = temperaturaMediaMovel -2 ; % Ajustar a posição Y acima da linha
text(x_position, y_position, sprintf('Temperatura Média: %.2f', temperaturaMediaMovel),'HorizontalAlignment', 'center', 'FontSize', 12, 'Color', [0.0, 0.0, 0.0]);
% Adicionar a linha de referência de temperatura camera Termica
yline(temperaturaCamera, ':', 'Color', [0.0, 1, 0.0], 'LineWidth', 1.5, 'DisplayName', 'Câmera Térmica');
x_position = length(colunaRelevante)/2; % Posição X para o texto
y_position = temperaturaCamera + 2 ; % Ajustar a posição Y acima da linha
text(x_position, y_position, sprintf('Temperatura Câmera Térmica: %.2f', temperaturaCamera),'HorizontalAlignment', 'center', 'FontSize', 12, 'Color', [0.0, 0.0, 0.0]);
% Adicionar a linha de referência de temperatura camera  termómetro no gráfico
yline(temperaturaTermometro, '-.', 'Color', [0.85,0.33,0.10], 'LineWidth', 1.5, 'DisplayName', 'Termômetro');
x_position = length(colunaRelevante)/2; % Posição X para o texto
y_position = temperaturaTermometro - 2; % Ajustar a posição Y acima da linha
text(x_position, y_position, sprintf('Temperatura Termômetro Mercúrio: %.2f', temperaturaTermometro),'HorizontalAlignment', 'center', 'FontSize', 12, 'Color', [0.0, 0.0, 0.0]);
ylim([30 60]);
title('Comparação dos Dados Originais e Filtrados (Plástico Transparente 200mm)');
xlabel('Tempo seg');
ylabel('Valor °C');
legend('show','FontSize',12);
grid on;
hold off;