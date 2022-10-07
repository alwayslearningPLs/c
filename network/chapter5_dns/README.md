# Learning DNS (Domain Name System)

## DNS Record types

- _A_: DNS record for IPv4.
- _AAAA_: DNS record for IPv6.
- _MX_: DNS record is used to return mail server information. For example, if you wanted to send an email to jorge@example.com, then the _MX_ record(s) for _example.com_ would indicate which mail server(s) receives emails for that domain.
- _TXT_: DNS record that can be used to store arbitrary information for a hostname. In practice, these are sometimes set to prove ownership of a domain name or to publish email sending guidelines. The _Sender Policy Framework_(SPF) standard uses _TXT_ records to declare which systems are allowed to send mail for a given domain name.
- _CNAME_(Canonical NAME): DNS record that can be used to provide an alias for a given name.
- `*` or _ALL_ or _ANY_: If this record is requested from a DNS server, then the DNS server returns all known record types for the current query. This query is not guaranteed to actually get all of the records for the requested domain (note that a DNS server is allowed to responsd with only the records in its cache).

Association:

| Record type | Type ID (decimal) |
| ----------- | ----------------- |
| A | 1 |
| AAAA | 28 |
| MX | 15 |
| TXT | 16 |
| CNAME | 5 |
| `*` | 255 |

## DNS Security

- _Domain Name System Security Extensions_ (DNSSEC): They are DNS extensions that provide data authentication. This authentication allows a DNS client to know that a given DNS reply is authentic, but it does not protect against eavesdropping.
- _DNS over HTTPS_ (DoH): It is a protocol that provides name resolution over HTTPS.

## Name/address translation functions

### Using getaddrinfo()

```c
int getaddrinfo (
  const char *node,
  const char *service,
  const struct addrinfo *hints,
  struct addrinfo **res
);

struct addrinfo {
  int               ai_flags;
  int               ai_family;
  int               ai_socktype;
  int               ai_protocol;
  socklen_t         ai_addrlen;
  struct sockaddr  *ai_addr;
  char             *ai_canonname;
  struct addrinfo  *ai_next;
};

// Relevanta fields are: ai_family, ai_socktype, ai_flags, ai_protocol.
```

Common flags on `ai_flags` inside `addrinfo` structure:

- `AI_NUMERICHOST`: It can be used to prevent name lookups.
- `AI_NUMERICSERV`: It can be used to only accept port numbers.
- `AI_ALL`: can be used to request both IPv4 and IPv6.
- `AI_ADDRCONFIG`: forces `getaddrinfo()` to only return addresses that match the family type of a configured interface on the local machine. eg: `AI_ADDRCONFIG | AI_ALL` prevents IPv6 addresses from being returned.
- `AI_PASSIVE`: can be used with `node = 0` to request the _wildcard address_.

### Using getnameinfo()

```c
int getnameinfo (
  const struct sockaddr *addr,
  socklen_t addrlen,
  char *host,
  socklen_t hostlen,
  char *serv,
  socklen_t servlen,
  int flags
);
```

Common flags on `flags` parameter:

- `NI_NAMEREQD`: requires to return a hostname and not an address. By default, `getnameinfo()` tries to return a hostname but returns an address if it can't. `NI_NAMEREQD` will cause an error to be returned if the hostname cannot be determined.
- `NI_DGRAM`: specifies that the service is UDP-based rather than TCP-based.
- `NI_NUMERICHOST`: requests that `getnameinfo()` returns the IP address and not a hostname.
- `NI_NUMERICSERV`: requests that `getnameinfo()` returns the port number and not a service name.
