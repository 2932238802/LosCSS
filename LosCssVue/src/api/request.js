import axios from 'axios'

const LosRequest = axios.create({
  baseURL: '/api',
  timeout: 15000
})

LosRequest.interceptors.response.use(
  response => response.data,
  error => {
    console.error('API 请求失败：', error)
    return Promise.reject(error)
  }
)

export default LosRequest
