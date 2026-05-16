import axios from 'axios'

const LosRequest = axios.create({
  baseURL: '/api',
  timeout: 15000
})

/**
 * 统一响应拦截：
 * 后端约定返回 { code, message, data }。
 * 业务成功（code === 0）时，调用方直接拿到 data；失败时统一 reject 一个带 message 的 Error。
 */

// 把后端返回的 body 转成业务友好的 Error，供 catch 拿到 message
function bodyToError(body, fallback) {
  const message = body?.message || fallback || '请求失败'
  const err = new Error(message)
  err.code = body?.code
  err.data = body?.data
  return err
}

LosRequest.interceptors.response.use(
  // HTTP 2xx：根据 body.code 决定成功/失败
  response => {
    const body = response.data

    if (!body || typeof body !== 'object' || !('code' in body)) {
      return body
    }

    if (body.code === 0) {
      return body.data
    }

    return Promise.reject(bodyToError(body))
  },
  // HTTP 非 2xx（4xx / 5xx / 网络错误）
  error => {
    // 后端虽然返回 4xx，但 body 仍是统一格式 { code, message, data }
    const body = error?.response?.data
    if (body && typeof body === 'object' && 'message' in body) {
      return Promise.reject(bodyToError(body))
    }

    // 没有 body 或不是 JSON：返回 axios 自己的 message
    console.error('API 请求失败：', error)
    return Promise.reject(error)
  }
)

export default LosRequest
