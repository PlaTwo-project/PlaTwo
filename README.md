# PlaTwo

## 🐳 راهنمای داکر (Docker)

### نحوه بیلد (Build)
برای ساخت ایمیج داکر پروژه، دستور زیر را در مسیر اصلی پروژه اجرا کنید:
```
docker build -t platwo-app .
```

### نحوه اجرا (Run)
برای تست و اجرای کانتینر در محیط بدون نمایشگر (Offscreen):

```
docker run --rm -it platwo-app ./PlaTwo -platform offscreen
```