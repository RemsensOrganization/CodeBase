# SSH ключи для GitHub на Windows (CMD)

## 🎯 Быстрая настройка (5 минут)

### 1. Генерация ключей (CMD **от администратора**)

```cmd
cd /d "C:\Users\%USERNAME%\.ssh"
ssh-keygen -t ed25519 -C "%USERNAME%@pc" -f id_ed25519 -N ""
```


### 2. ⚠️ КРИТИЧНО: Исправить права приватного ключа

**Через Проводник (100% работает):**

```
C:\Users\%USERNAME%\.ssh\id_ed25519
├── ПКМ → Свойства → Безопасность → Дополнительно
├── "Отключить наследование" → "Конвертировать"
├── Удалить ВСЕХ кроме %USERNAME%
├── %USERNAME% → Только "Чтение"
└── Применить
```

**Или CMD:**

```cmd
takeown /f id_ed25519
icacls id_ed25519 /inheritance:d /grant:r "%USERNAME%":R
```


### 3. Добавить публичный ключ в GitHub

```cmd
type id_ed25519.pub
```

```
ssh-ed25519 AAAAC3NzaC1lZDI1NTE5AAAAI... %USERNAME%@pc
```

→ **GitHub** → Settings → SSH and GPG keys → **New SSH key** → Вставить

### 4. Тест подключения

```cmd
ssh -T git@github.com
```

✅ **Успех:** `Hi %USERNAME%! You've successfully authenticated`

## 🚀 Использование

```cmd
git clone git@github.com:USERNAME/REPO.git
```


## ❌ Типичные ошибки и решения

| Ошибка | Решение |
| :-- | :-- |
| `Permissions for 'id_ed25519' are too open` | **Фикс прав через Проводник** |
| `Load key Permission denied` | **Правильные права только для тебя** |
| `Permission denied (publickey)` | **Публичный ключ не добавлен в GitHub** |

## 📋 Проверка состояния

```cmd
icacls id_ed25519
```

**Должно быть ТОЛЬКО:**

```
id_ed25519 %USERNAME%\(R)
```


***

**Автор:** VALERIY
**Дата:** 16 марта 2026
**Тестировано:** Windows 10/11, OpenSSH, GitHub**

