float4 VS(float4 Pos : POSITION) : SV_POSITION
{
    // Оставляем координаты точки без изменений
    return Pos;
} 

float4 PS(float4 Pos : SV_POSITION) : SV_Target
{
    // Возвращаем желтый цвет, непрозрачный (альфа == 1, альфа-канал не включен).
    float fLimiter = 100.0f;

    float dist = Pos.x * Pos.x + Pos.y * Pos.y;

    dist = (dist % fLimiter) / fLimiter;

    return float4(dist, 1-dist, dist, 1.0f);
}